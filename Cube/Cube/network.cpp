#include "network.h"
#include <string.h>



Network::Network(World * world)
{
	enet_initialize();

	m_isServer = Parametre::GetInstance().m_isServer;
	m_isConnected = false;
	m_lstClient = vector<Client>();

	m_EnetPeerServer = NULL;
	m_ENetAddress.host = ENET_HOST_ANY;
	m_ENetAddress.port = 1234;

	if (m_isServer)
		m_ENetHost = enet_host_create(&m_ENetAddress, 32, 2, 0, 0);
	else
		m_ENetHost = enet_host_create(NULL, 1, 2, 0, 0);

	if (!m_ENetHost)
	{
		std::cout << "Could't create ENETHost" << std::endl;
		exit(EXIT_FAILURE);
	}

	m_world = world;

}

Network::~Network()
{
	enet_host_destroy(m_ENetHost);
	enet_deinitialize();
}


bool Network::Fetch()
{
	while (enet_host_service(m_ENetHost, &m_ENetEvent, 0) > 0)
	{
		switch (m_ENetEvent.type)
		{
		case ENET_EVENT_TYPE_CONNECT:
			OnConnect();
			break;
		case ENET_EVENT_TYPE_RECEIVE:
			OnPacketReceive();
			break;
		case ENET_EVENT_TYPE_DISCONNECT:
			OnDisconnect();
		}
		return true;
	}
	return false;
}

void Network::Connect(const char *ip, uint16 port)
{
	if (m_isServer)
		return;

	Disconnect();

	enet_address_set_host(&m_ENetAddress, ip);
	m_ENetAddress.port = port;

	m_EnetPeerServer = enet_host_connect(m_ENetHost, &m_ENetAddress, 2, 0);
}

void Network::Disconnect()
{
	if (m_isServer || m_EnetPeerServer == NULL)
		return;

	enet_peer_disconnect(m_EnetPeerServer, 0);
}

void Network::Send(string data, bool reliable)
{
	ENetPacket * packet = enet_packet_create(data.c_str(), strlen(data.c_str()) + 1, (reliable) ? ENET_PACKET_FLAG_RELIABLE : ENET_PACKET_FLAG_UNSEQUENCED);

	// Broadcast a tous les client si on est un serveur
	// Send au serveur si on est un client
	if (m_isServer)
		enet_host_broadcast(m_ENetHost, (reliable) ? 1 : 0, packet);
	else if (m_EnetPeerServer)
		enet_peer_send(m_EnetPeerServer, (reliable) ? 1 : 0, packet);
}

void Network::SendTo(string data, bool reliable, ENetPeer * peer)
{
	ENetPacket * packet = enet_packet_create(data.c_str(), strlen(data.c_str()) + 1, (reliable) ? ENET_PACKET_FLAG_RELIABLE : ENET_PACKET_FLAG_UNSEQUENCED);

	enet_peer_send(peer, (reliable) ? 1 : 0, packet);
}

vector<Client> Network::GetClient()
{
	return m_lstClient;
}

bool Network::IsConnected()
{
	return m_isConnected;
}

void Network::OnConnect()
{
	std::cout << "Connected event happens" << std::endl;
	m_isConnected = true;

	// les client efface leur map
	if (!m_isServer)
	{
		m_world->InitMap(0);
		m_world->SpawnPlayer();
	}
}

void Network::OnDisconnect()
{
	std::cout << "Disconnected event happens" << std::endl;
	m_isConnected = false;

	m_EnetPeerServer = NULL;
	if (!m_isServer)
	{
		m_world->InitMap(42);
		m_world->SpawnPlayer();
	}

	// Force to recreate the list
	m_lstClient.clear();
}
void Network::OnPacketReceive()
{
	string packet = (char *)m_ENetEvent.packet->data;
	//if (packet[0] != 'p')
	//	std::cout << packet << std::endl;

	if (packet.find("map") == 0)
	{
		std::istringstream stream(packet);

		string null;
		float x, y, z;
		int b;

		stream >> null >> x >> y >> z >> b;

		Vector3<float> chunkPos(floor(x / CHUNK_SIZE_X), 0, floor(z / CHUNK_SIZE_Z));
		Chunk * chunk = m_world->ChunkAt(chunkPos.x, chunkPos.z);

		if (chunk)
			chunk->SetBlock(x - (chunkPos.x * CHUNK_SIZE_X), y, z - (chunkPos.z * CHUNK_SIZE_X), b, ')');

		// le serveur broadcast le changement a tous les clients
		if (m_isServer)
			Send(packet, true);
	}
	else if (packet.find("RequestChunk") == 0)
	{
		if(!m_isServer)
			return;

		std::istringstream stream(packet);

		string null;
		int posX, posZ;
		stream >> null >> posX >> posZ;

		Chunk* chunk = m_world->ChunkAt(posX,posZ);

		if (chunk != NULL)
		{
			if (chunk->m_iscreated == false)
				m_world->InitChunk(posX,posZ);

			std::stringstream toSendStream;

			toSendStream << "ThisIsChunk " << posX << " " << posZ << " ";

			for (int x = 0; x < CHUNK_SIZE_X; ++x)
				for (int z = 0; z < CHUNK_SIZE_Z; ++z)
					for (int y = 0; y <= CHUNK_SIZE_Y; y++)
					{
						toSendStream << (int)chunk->GetBlock(x, y, z) << " ";
					}
			SendTo(toSendStream.str(), 1, m_ENetEvent.peer);
		}
	}
	else if (packet.find("ThisIsChunk") == 0)
	{
		std::istringstream stream(packet);

		string null;
		int posX, posZ, block;
		stream >> null >> posX >> posZ;

		Chunk* chunk = m_world->ChunkAt((float)posX, (float)posZ);
		chunk->m_iscreated = true;

		for (int x = 0; x < CHUNK_SIZE_X; ++x)
			for (int z = 0; z < CHUNK_SIZE_Z; ++z)
				for (int y = 0; y <= CHUNK_SIZE_Y; y++)
				{
					stream >> block;
					if (block >= 0 && block < NUMBER_OF_BLOCK)
					{
						chunk->SetBlock(x, y, z, block, ' ');
					}
				}
	}
	else if (packet.find("player") == 0)
	{
		Client tmpClient;
		tmpClient.FromString(packet);

		// Update le client dans la liste
		bool isFound = false;
		for (auto&& c : m_lstClient)
			if (c.name == tmpClient.name)
			{
				c = tmpClient;
				isFound = true;
			}

		// S'il n'est pas deja dans la liste on l'ajoute
		if (!isFound)
			m_lstClient.push_back(tmpClient);
	}
}

#include "network.h"

Network::Network()
{
	enet_initialize();

	m_isServer = Parametre::GetInstance().m_isServer;
	m_lstClient = vector<Client>();

	m_EnetPeerServer = NULL;
	m_ENetAddress.host = ENET_HOST_ANY;
	m_ENetAddress.port = 1234;

	if(m_isServer)
		m_ENetHost = enet_host_create(&m_ENetAddress, 32, 2, 0, 0);
	else
		m_ENetHost = enet_host_create(NULL, 1, 2, 0, 0);

	if (!m_ENetHost)
	{
		std::cout << "Could't create ENETHost" << std::endl;
		exit(EXIT_FAILURE);
	}

}

Network::~Network()
{
	enet_host_destroy(m_ENetHost);
	enet_deinitialize();
}

bool Network::Fetch()
{
	if (enet_host_service(m_ENetHost, &m_ENetEvent, 0) > 0)
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

bool Network::Send(string data)
{
	ENetPacket * packet = enet_packet_create(data.c_str(), strlen(data.c_str()) + 1, 0);

	// Broadcast a tous les client si on est un serveur
	// Send au serveur si on est un client
	if (m_isServer)
		enet_host_broadcast(m_ENetHost, 1, packet);
	else if(m_EnetPeerServer)
		enet_peer_send(m_EnetPeerServer, 0, packet);

	enet_host_flush(m_ENetHost);

	return false;
}

vector<Client> Network::GetClient()
{
	return m_lstClient;
}

void Network::OnConnect()
{
	std::cout << "Connected event happens" << std::endl;
}

void Network::OnDisconnect()
{
	std::cout << "Disconnected event happens" << std::endl;

	// Force to recreate the list
	m_lstClient.clear();
}

void Network::OnPacketReceive()
{
	//std::cout << "Packet Received: "<< m_ENetEvent.packet->data << std::endl;
	Client tmpClient;

	try
	{
		tmpClient.FromString(std::stringstream((char *)m_ENetEvent.packet->data));
	}
	catch (const std::exception ex)
	{
		std::cout << ex.what() << std::endl;
		return;
	}

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

	enet_packet_destroy(m_ENetEvent.packet);
}



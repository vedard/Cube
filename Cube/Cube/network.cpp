#include "network.h"

Network::Network()
{
	enet_initialize();

	m_isServer = Parametre::GetInstance().m_isServer;
	m_lstClient = vector<Client>();

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
	if (enet_host_service(m_ENetHost, &m_ENetEvent, 0))
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

	ENetPeer * peer = enet_host_connect(m_ENetHost, &m_ENetAddress, 2, 0);

	if (peer == NULL)
		std::cout << "Couldn't connect to the server" << std::endl;
}

void Network::Disconnect()
{
	if (m_isServer)
		return;
	
	for (auto c : m_lstClient)
		enet_peer_disconnect(c.peer, 0);

	m_lstClient.clear();
}

bool Network::Send(string data)
{
	for (auto c : m_lstClient)
	{
		ENetPacket * packet = enet_packet_create(data.c_str(), strlen(data.c_str()) + 1, ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(c.peer, 0, packet);
		enet_host_flush(m_ENetHost);
	}

	return false;
}

void Network::OnConnect()
{
	std::cout << "Connected event happens" << std::endl;
	m_lstClient.push_back(Client(m_ENetEvent.peer, "test"));
}

void Network::OnDisconnect()
{
	std::cout << "Disconnected event happens" << std::endl;
}

void Network::OnPacketReceive()
{
	std::cout << "Packet Received: "<< m_ENetEvent.packet->data << std::endl;
	enet_packet_destroy(m_ENetEvent.packet);
}



#ifndef NETWORK_H__
#define NETWORK_H__

#include <enet/enet.h>
#include <vector>
#include <string.h>
#include <sstream>
#include "vector3.h"
#include "networkevent.h"
#include "parametre.h"

using std::vector;
using std::string;

struct Client
{
	float x, y, z, h, v;
	string name;

	string ToString()
	{
		return	name + " "
			+ std::to_string(x) + " "
			+ std::to_string(y) + " "
			+ std::to_string(z) + " "
			+ std::to_string(h) + " "
			+ std::to_string(v) + " ";
	
	}
	void FromString(const std::string& str)
	{
		std::istringstream data(str);
		data >> name >> x >> y >> z >> h >> v;
	}
};

class Network : public NetworkEvent
{

public:
	Network();
	virtual ~Network();
	bool Fetch();
	void Connect(const char * ip, uint16 port);
	void Disconnect();
	bool Send(string data);
	vector<Client> GetClient();

private:

	// Network parameter
	bool m_isServer;

	// List of client connected to the server
	vector<Client> m_lstClient;

	//ENet
	ENetHost *	m_ENetHost;
	ENetEvent	m_ENetEvent;
	ENetAddress	m_ENetAddress;
	ENetPeer *	m_EnetPeerServer;

	// Hérité via NetworkEvent
	virtual void OnConnect() override;
	virtual void OnDisconnect() override;
	virtual void OnPacketReceive() override;

};


#endif

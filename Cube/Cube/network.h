#ifndef NETWORK_H__
#define NETWORK_H__

#include <enet/enet.h>
#include <vector>
#include "vector3.h"
#include "networkevent.h"
#include "parametre.h"

using std::vector;
using std::string;

struct Client
{
	Vector3<float> pos;
	string name;
	ENetPeer * peer;

	Client(ENetPeer * a_peer, string a_name) : peer(a_peer), name(a_name) {}
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

private:

	// Network parameter
	bool m_isServer;

	// List of connected client
	vector<Client> m_lstClient;

	//ENet
	ENetHost *	m_ENetHost;
	ENetEvent	m_ENetEvent;
	ENetAddress	m_ENetAddress;

	// Hérité via NetworkEvent
	virtual void OnConnect() override;
	virtual void OnDisconnect() override;
	virtual void OnPacketReceive() override;

};


#endif
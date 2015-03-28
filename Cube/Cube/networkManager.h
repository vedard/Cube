#ifndef NETWORK_MANAGER_H__
#define NETWORK_MANAGER_H__

#include "define.h"
#include <iostream>

class NetworkManager
{
public:
	NetworkManager();
	~NetworkManager();
	
	void Send(const std::string &packet);
	std::string Receive();

private:
	bool InitSocket();
	bool SetSocketNonBlocking();

private:
	int m_socketHandle;
	sockaddr_in m_address;
};

#endif
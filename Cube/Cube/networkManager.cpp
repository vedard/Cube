#include "networkManager.h"



NetworkManager::NetworkManager()
{
	//For windows init the socket layer (WSADATA)
	if (!InitSocket())
	{
		std::cout << "Error WSADATA" << std::endl;
		exit(1);
	}

	//Get socket handle
	m_socketHandle = socket(AF_INET, SOCK_DGRAM, 0);
	if (m_socketHandle <= 0)
	{
		std::cout << "failed to create socket" << std::endl;
		exit(1);
	}

	//Address
	m_address.sin_family = AF_INET;
	m_address.sin_addr.s_addr = htonl(INADDR_ANY);
	m_address.sin_port = htons(6667);

	//Bind
	if (bind(m_socketHandle, (const sockaddr*)&m_address, sizeof(sockaddr_in)) < 0)
	{
		std::cout << "failed to bind socket" << std::endl;
		exit(1);
	}

	//Set the socket to non-blocking mode
	if (SetSocketNonBlocking() == false)
	{
		std::cout << "failed to set non-blocking" << std::endl;
		exit(1);
	}

	std::cout << "	Network is ok" << std::endl;

}


void NetworkManager::Send(const std::string &packet)
{
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(6666);
	addr.sin_addr.s_addr = htonl(
        ( (unsigned long) 127 << 24 ) |
        ( (unsigned long) 0 << 16 ) |
        ( (unsigned long) 0 <<  8 ) |
        ( (unsigned long) 1 ));
	sendto(m_socketHandle, packet.c_str(), packet.length(), 0, (const sockaddr*)&addr, sizeof(addr));
}

std::string NetworkManager::Receive()
{
	sockaddr_in client;
	int addrLen = sizeof(client);
	char buffer[2048];
	int count = recvfrom(m_socketHandle, buffer, sizeof(buffer) - 1, 0, (sockaddr*)&client, (socklen_t*)&addrLen);

	if (count < 0)	
		return "";
	
	buffer[count] = '\0';

	//std::cout << "Recu de "<< client.sin_addr.s_addr <<": " << buffer << std::endl;
	return std::string(buffer);
}


NetworkManager::~NetworkManager()
{
#if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
	close(m_socketHandle);
#elif PLATFORM == PLATFORM_WINDOWS
	closesocket(m_socketHandle);
	WSACleanup();
#endif

}

bool NetworkManager::InitSocket()
{

#if (PLATFORM == PLATFORM_WINDOWS)

	WSADATA WsaData;
	return WSAStartup(MAKEWORD(2, 2), &WsaData) == NO_ERROR;

#elif PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
	return true;
#endif

}

bool NetworkManager::SetSocketNonBlocking()
{

#if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX

	int nonBlocking = 1;
	if (fcntl(m_socketHandle, F_SETFL, O_NONBLOCK, nonBlocking) == -1)
		return false;
	else
		return true;


#elif PLATFORM == PLATFORM_WINDOWS

	DWORD nonBlocking = 1;
	if (ioctlsocket(m_socketHandle, FIONBIO, &nonBlocking) != 0)
		return false;
	else
		return true;

#endif

}


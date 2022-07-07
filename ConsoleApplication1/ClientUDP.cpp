#include "stdafx.h"
#include "ClientUDP.h"


ClientUDP::ClientUDP(const char* ip, unsigned short port)
{
	//create socket
	if ((mySocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
	{
		std::cout << "socket() failed with error code : " << WSAGetLastError() << std::endl;
		init = false;

		return;
	}

	//setup address structure
	memset((char*)&sockaddr, 0, sizeof(sockaddr));
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(port);
	sockaddr.sin_addr.S_un.S_addr = INADDR_ANY;//Can't bind to an address outside the local network

	if (bind(mySocket, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) == SOCKET_ERROR)
	{
		std::cout << "Bind failed with error code : " << WSAGetLastError() << std::endl;
		init = false;

		return;
	}

	unsigned long opt = 1;

	int erreur = ioctlsocket(mySocket, FIONBIO, &opt);

	if (erreur != NO_ERROR)
	{
		std::cout << "Erreur ioctlsocket : " << erreur << " " << WSAGetLastError() << std::endl;
	}

	init = true;
}

ClientUDP::~ClientUDP()
{
	closesocket(mySocket);
}

bool ClientUDP::recv(char* buff, unsigned int size)
{
	int sockaddrLen = sizeof(sockaddr);

	if (recvfrom(mySocket, buff, size, 0, (struct sockaddr*)&sockaddr, &sockaddrLen) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != 10035)//Ressource not yet available, due to nonblock socket
		{
			std::cout << "recvfrom() failed with error code : " << WSAGetLastError() << std::endl;
		}
		
		return false;
	}

	return true;
}
#include "stdafx.h"
#include "ServerUDP.h"


ServerUDP::ServerUDP(unsigned short _port)
{
	port = _port;
	mySocket = socket(AF_INET, SOCK_DGRAM, 0);

	//Create a socket
	if(mySocket == INVALID_SOCKET)
	{
		std::cout << "Could not create socket : " << WSAGetLastError() << std::endl;
		init = false;
	}

	sockaddr.sin_family = AF_INET;
	sockaddr.sin_addr.s_addr = INADDR_ANY;
	sockaddr.sin_port = htons(port);

	//Bind
	/*if (bind(mySocket, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) == SOCKET_ERROR)
	{
		std::cout << "Bind failed with error code : " << WSAGetLastError() << std::endl;
		init = false;
	}*/

	unsigned long opt = 1;

	int erreur = ioctlsocket(mySocket, FIONBIO, &opt);

	if (erreur != NO_ERROR)
	{
		std::cout << "Erreur ioctlsocket : " << erreur << " " << WSAGetLastError() << std::endl;
	}

	init = true;
}

ServerUDP::~ServerUDP()
{
	closesocket(mySocket);
}

void ServerUDP::addClient(IN_ADDR addr)
{ 
	struct sockaddr_in newSockaddr;

	memset((char*)&newSockaddr, 0, sizeof(newSockaddr));
	newSockaddr.sin_family = AF_INET;
	newSockaddr.sin_port = htons(port);
	newSockaddr.sin_addr = addr;

	clientsAddr.push_back(newSockaddr);
}

bool ServerUDP::send(char* buff, unsigned int size)
{
	bool success = true;

	for (int i = 0; i < clientsAddr.size(); i++)
	{
		int sockaddrLen = sizeof(clientsAddr[i]);

		if (sendto(mySocket, buff, size, 0, (struct sockaddr*)&clientsAddr[i], sockaddrLen) == SOCKET_ERROR)
		{
			if (WSAGetLastError() != 10035)//Ressource not yet available, due to nonblock socket
			{
				std::cout << "recvfrom() failed with error code : " << WSAGetLastError() << std::endl;
			}

			success = false;
		}
	}

	return success;
}
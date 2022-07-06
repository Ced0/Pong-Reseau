#pragma once


#include <vector>
#include <thread>
#include <mutex>

#include <iostream>

#ifdef __linux__
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/ioctl.h>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>

#define SOCKET int
#define SOCKADDR_IN sockaddr_in
#define INVALID_SOCKET -1
#define WSAGetLastError() errno
#define closesocket(s) close(s)

#elif _WIN32
#ifndef _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif // !_WINSOCK_DEPRECATED_NO_WARNINGS

//#include <windows.h>
#include <winsock2.h>
#endif

class ClientUDP
{
public:
	ClientUDP(const char* ip, unsigned short port);

	~ClientUDP();

	inline bool isValid() { return init; }

	bool recv(char* buff, unsigned int size);

private:
	SOCKET mySocket;
	struct sockaddr_in sockaddr;
	bool init;
};


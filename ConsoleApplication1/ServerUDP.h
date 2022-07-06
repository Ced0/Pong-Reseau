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
//#include <windows.h>
#include <winsock2.h>
#endif

class ServerUDP
{
public:
	ServerUDP(unsigned short port);

	~ServerUDP();

	inline bool isValid() { return init; }

	void addClient(IN_ADDR addr);

	bool send(char* buff, unsigned int size);

private:
	SOCKET mySocket;
	struct sockaddr_in sockaddr;
	std::vector<SOCKADDR_IN> clientsAddr;
	bool init;
	unsigned short port;
};


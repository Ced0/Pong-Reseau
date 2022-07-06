#ifndef CLIENT_H
#define CLIENT_H



//#include "socketclass.h"
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
    #define SOCKET_ERROR -1

#elif _WIN32
#ifndef _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif // !_WINSOCK_DEPRECATED_NO_WARNINGS

    //#include <windows.h>
    #include <winsock2.h>
    #include <ws2tcpip.h>
#endif



#include <string.h>


using namespace std;

class Client
{
public:
    Client();
    Client(const char* ip,  unsigned int port);
    ~Client();

    bool connectSocket(const char* ip,  unsigned int port);

    bool envoie(const char* buffer, int size);

    bool reception(char *buffer, int size);

    inline bool isConnected() { return connected; }

protected:

    SOCKET mySocket;
    SOCKADDR_IN sock_in;
    bool connected = false;
};

#endif // CLIENT_H

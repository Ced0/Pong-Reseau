#ifndef SERVEUR_H
#define SERVEUR_H

#include <vector>
#include <thread>
#include <mutex>
#include "Reseau.h"

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

using namespace std;

class Serveur
{
public:
    explicit Serveur(Game* structGame);

    ~Serveur();

    void listenerThread(unsigned short port);

    void ClientThread(SOCKET current_client, Game* structGame);

    void launchListenerThread(unsigned short port);
     
    inline bool isConnected() { return connected; }

    inline IN_ADDR getClient(int i) { return clientsAddr[i].sin_addr; };
    inline int getClientSize() { return clientsAddr.size(); };

    void join();

private:
    std::vector<unsigned long long> adresse;
    vector<std::thread> threads;
    vector<SOCKADDR_IN> clientsAddr;
    Game* structGame;
    int nClient = 0;
    bool connected = false;
};

#endif // SERVEUR_H

#include "stdafx.h"
#include "client.h"

Client::Client()
{
    int erreur;

    mySocket = socket(AF_INET,SOCK_STREAM,0);
    if(mySocket == INVALID_SOCKET)
        std::cout << "Erreur création de la socket : " << WSAGetLastError() << endl;

    int temp = 1;

    erreur = setsockopt(mySocket, IPPROTO_TCP, TCP_NODELAY,(char*)& temp, sizeof(temp));

    if(erreur != 0)
        std::cout << "Erreur option socket : " << erreur << " " << WSAGetLastError() << endl;
}

Client::Client(const char* ip,  unsigned int port)
{
    int erreur;
    /*#ifdef _WIN32
    WSADATA init_win32;


    erreur = WSAStartup(MAKEWORD(2,2), &init_win32);
    if(erreur != 0)
        std::cout << "Erreur initialisation : " << erreur << " " << WSAGetLastError() << endl;

    #endif*/

    mySocket = socket(AF_INET,SOCK_STREAM,0);
    if(mySocket == INVALID_SOCKET)
        std::cout << "Erreur création de la socket : " << WSAGetLastError() << endl;

    int temp = 1;

    erreur = setsockopt(mySocket, IPPROTO_TCP,TCP_NODELAY,(char*)& temp, sizeof(temp));

    if(erreur != 0)
        std::cout << "Erreur option socket : " << erreur << " " << WSAGetLastError() << endl;

    connectSocket(ip, port);
}

Client::~Client()
{
    int erreur = closesocket(mySocket);
    if(erreur != 0)
        std::cout << "Erreur closing socket : " << erreur << " " << WSAGetLastError() << endl;
}

bool Client::connectSocket(const char* ip, unsigned int port)
{
    sock_in.sin_family = AF_INET;
    sock_in.sin_addr.s_addr = inet_addr(ip);
    //InetPton(AF_INET, ip, &sock_in.sin_addr.s_addr);
    sock_in.sin_port = htons(port);
    int erreur = connect(mySocket, (struct sockaddr *) &sock_in, sizeof(sock_in));

    if(erreur != 0){
        std::cout << "Erreur connection : " << erreur << " " << WSAGetLastError() << endl;
        return false;
    }

    unsigned long opt = 1;

    erreur = ioctlsocket(mySocket, FIONBIO, &opt);

    if (erreur != NO_ERROR)
    {
        std::cout << "Erreur ioctlsocket : " << erreur << " " << WSAGetLastError() << endl;
        return false;
    }
        
    std::cout << "Connection effectuer " << endl;
    std::cout << "Client port : " << port << endl;
    connected = true;
    return true;
}

bool Client::envoie(const char *buffer, int size)
{
    int n = send(mySocket, buffer, size, 0);
    if(n == SOCKET_ERROR)
    {
        std::cout << "Erreur envoie TCP : " << n << " " << WSAGetLastError() << endl;
        return false;
    }

    //std::cout << "Envoie effectuer" << endl;
    return true;
}

bool Client::reception(char *buffer, int size)
{
    int n = recv(mySocket, buffer, size, 0);
    if(n == SOCKET_ERROR)
    {
        if (WSAGetLastError() != 10035)//Ressource not yet available, due to nonblock socket
        {
            std::cout << "Erreur reception TCP : " << n << " " << WSAGetLastError() << endl;
        }
        
        return false;
    }else{
        //std::cout << "Reception effectuer " << n << endl;
        return true;
    }
}


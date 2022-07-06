#include "stdafx.h"
#include "wsainit.h"

#ifdef _WIN32

WSAInit::WSAInit()
{
    WSADATA init_win32;

    int erreur = WSAStartup(MAKEWORD(2,2), &init_win32);
    if(erreur != 0)
    {
        std::cout << "Erreur initialisation : " << erreur << " " << WSAGetLastError() << std::endl;
    }else{
        std::cout << "WSA OK\n";
    }
}

WSAInit::~WSAInit()
{
    WSACleanup();
}
#endif

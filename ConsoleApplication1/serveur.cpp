#include "stdafx.h"
#include "serveur.h"

Serveur::Serveur(Reseau* _structReseau)
{
    //threads.push_back(std::thread(&Serveur::listenerThread, this, port));
    structReseau = _structReseau;
}

Serveur::~Serveur()
{
    structReseau->_exit = true;

    for (int i = 0; i < threads.size(); i++)
    {
        threads[i].join();
    }
}

void Serveur::join()
{
    for (int i = 0; i < threads.size(); i++)
    {
        threads[i].join();
    }
}

void Serveur::launchListenerThread(unsigned short port)
{
    if(threads.size() > 0)
    {
        std::cout << "Listener thread already active !";
    }else{
        threads.push_back(std::thread(&Serveur::listenerThread, this, port));
    }

}

void Serveur::listenerThread(unsigned short port)
{
    SOCKET socket_ecoute;//identifiant de la socket d'ecoute du cote serveur
    SOCKADDR_IN information_sur_la_source;
    //pour le thread

    //ouverture de la socket
    socket_ecoute = socket(AF_INET,SOCK_STREAM,0);

    if(socket_ecoute == INVALID_SOCKET){
        std::cout << "desole, peux pas creer la socket du a l'erreur : " << WSAGetLastError() << endl;
        return;
    }else{
        std::cout << "socket : OK" << endl;
    }

    int tempo = 1;
    int erreur = setsockopt(socket_ecoute, IPPROTO_TCP,TCP_NODELAY,(char*)&tempo, sizeof(tempo));

    if(erreur != 0) std::cout << "\n desole peux pas configurer cette option du a l'erreur : " << erreur << "->" << WSAGetLastError() <<endl;
    else std::cout << "setsockopt(): OK" << endl;

    information_sur_la_source.sin_family = AF_INET;
    information_sur_la_source.sin_addr.s_addr = INADDR_ANY;//ecoute sur toutes les IP locales
    information_sur_la_source.sin_port = htons(port);//ecoute sur le port
    erreur = bind(socket_ecoute, (struct sockaddr*)&information_sur_la_source, sizeof(information_sur_la_source));

    if(erreur != 0)
    {
        std::cout << "\nDeolse, je ne peux pas ecouter sur ce port : " << erreur << WSAGetLastError() << endl;
        exit(1);
    }else{
        std::cout << "bind(): OK" << endl;
    }


    unsigned long nonBlock = 1;

    #ifdef __linux__
    if(fcntl(socket_ecoute, F_SETFL,O_NONBLOCK))
    {
        qDebug() << "fcntl() failed with error : " << WSAGetLastError() << endl;
    }else{
        qDebug() << "fcntl()  success !" << endl;
    }
    #elif _WIN32
    if(ioctlsocket(socket_ecoute, FIONBIO, &nonBlock))
    {
        std::cout << "ioctlsocket() failed with error : " << WSAGetLastError() << endl;
    }else{
        std::cout << "ioctlsocket()  success !" << endl;
    }
    #endif


    //ecoute sur le socket d'ecoute
    do{
        erreur = listen (socket_ecoute, 10);
    }while(erreur != 0);

    std::cout << "listen(): OK" << endl;
    SOCKET socket_travail;//identifiant de la nouvelle socket client...cote serveur
//acceptation de la demande d'ouverture de session
    std::cout << "\nAttente de la reception de damande d'ouverture de nouvelle session TCP(SYN):\n";


    tempo = sizeof(information_sur_la_source);//passer par une variable afin d'utiliser un pointeur

    while(structReseau->_exit == false && nClient < structReseau->maxClient)
    {
        #ifdef __linux__
        socket_travail = accept(socket_ecoute,(struct sockaddr*)&information_sur_la_source, (socklen_t*)&tempo);
        #elif _WIN32
        socket_travail = accept(socket_ecoute,(struct sockaddr*)&information_sur_la_source, &tempo);
        #endif

        if(socket_travail == INVALID_SOCKET){
            //qDebug() << "\nDesole, je peux pas accepter la session TCP du a l'erreur : " << WSAGetLastError() << endl;
        }else{
            std::cout << "accpet() : OK" << endl;
            threads.push_back(std::thread(&Serveur::ClientThread, this, socket_travail, structReseau));
            clientsAddr.push_back(information_sur_la_source);
            nClient++;
            //emit newConnection();
        }
    }

    std::cout << "Max clients connected" << endl;

    connected = true;
    structReseau->connecte = true;

    shutdown(socket_ecoute, 2);
    closesocket(socket_ecoute);

}

void Serveur::ClientThread(SOCKET current_client, Reseau* structReseau)
{
    std::cout << "creation du thread associe a un client ..\n";
    //recupere le socket passe en tant que parametre
    char buf[10] = {0};
    //buffer pour les données a envoyer
    char sendData[10] = {0};
    int res;
    //boucle de reception des données du client
    while(structReseau->_exit == false)
    {

        memset(buf, 0, 10);
        res = recv(current_client, buf, sizeof(buf), 0);//reception de la commanded'un client

        //Sleep(10);
        if (res == -1)
        {
            //std::cout << "recv() failed with error : " << WSAGetLastError() << endl;
        }else if(res == 0)
        {
            //MessageBoxA(0, "erreur","erreur", MB_OK);
            std::cout << "Erreur";
            closesocket(current_client);
            std::terminate();

        }else if(strstr(buf,"\r\n"))//commande parasite envoyee par le client
        {
            std::cout << ".\n";

        }else if(strstr(buf, "bye"))
        {
            std::cout << "--> commande 'quitter' recue ..\n";//<<endl;
            //strcpy_s(sendData, "bye bye...\r\n");
            //Sleep(10);
            //send(current_client, sendData, sizeof(sendData), 0);
            //fermer le  socket associe avec ce client et terminer le thread en cours
            closesocket(current_client);
            //std::terminate();

            structReseau->mute.lock();
            structReseau->connecte = false;
            structReseau->_exit = true;
            structReseau->mute.unlock();

        }else{//We recieved the data
            bool press;
            unsigned char key;

            if ((buf[0] & 1) == 1)
            {
                press = true;
                std::cout << "pressed received" << endl;
            }
            else {
                std::cout << "released received" << endl;
                press = false;
            }
            
            key = buf[1];

            structReseau->mute.lock();
            structReseau->colliders_kinematic[1].keyboardEvents(key, press);
            structReseau->mute.unlock();
        }
    }

    //Tell clients we are leaving
    if (structReseau->connecte == true)
    {
        strcpy_s(sendData, "bye");
        send(current_client, sendData, 4, 0);

        
    }

    //emit connectionClose();
    shutdown(current_client, 2);
    closesocket(current_client);
}


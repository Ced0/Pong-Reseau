#include "stdafx.h"
#include "Reseau.h"

void receptionTCP(struct Reseau* structReseau, Client* client) //Recepetion avec le client
{
	char buffer[4] = "";

	while (strstr(buffer, "bye") == false && structReseau->_exit == false)
	{
		client->reception(buffer, sizeof(buffer));
	}

	structReseau->_exit = true;
	

	/*sf::SocketSelector selector;

	selector.add(*(structReseau->tcpSocket));
	selector.add(*(structReseau->udpSocket));

	sf::Packet packet;

	sf::IpAddress udpIp;
	unsigned short udpPort;

	do {

		if (selector.wait(sf::seconds(1)))//On attend de recevoir un message, et on vérifie régulièrement que l'on ne c'est pas déconnecter
		{
			if (selector.isReady(*(structReseau->tcpSocket)))
			{
				if (structReseau->tcpSocket->receive(packet) != sf::Socket::Done)//Reception du paquet
				{
					std::cout << "Erreur reception \n";
				}
				else {
					if (structReseau->_exit == false)//Evite de se reconnecter si on est sur le point de se deconnecter
						structReseau->_exit = receiveGame(packet, structReseau);
				}
			}
			else {
				if (structReseau->udpSocket->receive(packet, udpIp, udpPort) != sf::Socket::Done)//Reception du paquet
				{
					std::cout << "Erreur reception \n";
				}
				else {
					if (structReseau->_exit == false)//Evite de se reconnecter si on est sur le point de se deconnecter
						structReseau->_exit = receiveGame(packet, structReseau);
				}
			}

		}

	} while (!structReseau->_exit);*/

}

void envoieServeur(bool playing, struct Reseau structReseau)//Fonction d'envoie Serveur vers Client
{

	/*sf::Packet packet;

	if (playing == true)
	{
		sendGame(packet, structReseau);

		if (structReseau.udpSocket->send(packet, structReseau.tcpSocket->getRemoteAddress(), 5300) != sf::Socket::Done && structReseau._exit != true)//Si notre serveur c'est déja déjà déconnecter, on ne vérifie si le message a bien été envoyer
		{
			std::cout << "Erreur envoie \n";
		}
	}
	else {
		packet << false;

		if (structReseau.tcpSocket->send(packet) != sf::Socket::Done && structReseau._exit != true)//Si notre serveur c'est déja déjà déconnecter, on ne vérifie si le message a bien été envoyer
		{
			std::cout << "Erreur envoie \n";
		}
	}*/



}

void envoieClient(bool playing, struct Reseau structReseau, Client* client, unsigned char key, bool press)// Fonction d'envoie Client vers Serveur
{
	char* buffer;
	int sizePack;

	if (playing == true)
	{
		//sendInput(packet, press, key);

		sizePack = 2;
		buffer = new char[sizePack];

		unsigned char b = (press << 0);

		*(buffer) = b;
		*(buffer + 1) = key;

		client->envoie(buffer, sizePack);
	}
	else {
		sizePack = 3;
		buffer = new char[sizePack];

		std::memcpy(buffer, "bye", 3);

		client->envoie(buffer, sizePack);
	}



	if (client->envoie(buffer, sizePack) != true && structReseau._exit != true)//Si notre serveur c'est déja déjà déconnecter, on ne vérifie si le message a bien été envoyer
	{
		std::cout << "Erreur envoie \n";
	}

	delete[] buffer;

}

void clientServeur(bool* connecte, struct Reseau* structReseau)//Reception "serveur"
{
	/*sf::SocketSelector selector;

	sf::TcpListener* listener = new sf::TcpListener;

	//selector.add(*(structReseau->udpSocket));

	sf::Packet packet;

	sf::IpAddress udpIp;
	unsigned short udpPort;

	if (listener->listen(5300) != sf::Socket::Done)//On écoute si il ya de nouvelle connexion
	{
		std::cout << "Erreur listener \n";
		return;
	}
	else {
		selector.add(*listener);
		std::cout << "En attente de connexion \n";
	}

	do {

		if (selector.wait(sf::seconds(1)))//On attend un message ou une connection
		{
			//std::cout << *connecte;
			if (*connecte == false)
			{
				if (selector.isReady(*listener))//Si c'est une connection
				{

					if (listener->accept(*(structReseau->tcpSocket)) != sf::Socket::Done)
					{
						std::cout << "Erreur connexion \n";

					}
					else {
						std::cout << "Connexion réussi \n";//Supression du listener, et ajout de notre socket au selector

						*connecte = true;
						listener->close();
						selector.remove(*listener);
						delete listener;
						selector.add(*(structReseau->tcpSocket));
					}
				}
			}
			else {

				if (selector.isReady(*(structReseau->tcpSocket)))//Si c'est une connection
				{

					//std::cout<<"Recu ! \n";
					if (structReseau->tcpSocket->receive(packet) != sf::Socket::Done)//Reception du paquet
					{
						std::cout << "Erreur reception \n";
					}
					else {
						structReseau->_exit = receiveInput(packet, structReseau);

						if (structReseau->_exit)
							*connecte = false;
					}

				}/*else{
					if (structReseau->udpSocket->receive(packet, udpIp, udpPort) != sf::Socket::Done)//Reception du paquet
					{
						std::cout << "Erreur reception \n";
					}else{
						structReseau->_exit = receiveInput(packet, structReseau);

						if(structReseau->_exit)
							*connecte = false;
					}
				}*
			}

		}

	} while (!structReseau->_exit);*/


}

//TO BE CORRECTED
void sendGame(struct Reseau *structReseau, ServerUDP* server)//Ecriture du packet Serveur vers Client
{
	int sizePack = sizeof(int) * 8 + 1;

	char buffer[sizeof(int) * 8 + 1];

	int offset = 0;

	unsigned char b = (structReseau->snd_paddle << 0) + (structReseau->snd_goal << 1) + (structReseau->snd_wall << 2);

	memcpy(buffer, &b, sizeof(int));
	offset++;

	for (int i = 0; i < 3; i++)
	{
		int a = structReseau->colliders_kinematic[i].get_x();
		memcpy(buffer + offset, &a, sizeof(int));
		offset += sizeof(int);

		a = structReseau->colliders_kinematic[i].get_y();
		memcpy(buffer + offset, &a, sizeof(int));
		offset += sizeof(int);
	}

	memcpy(buffer + offset, &structReseau->score1, sizeof(int));
	offset += sizeof(int);
	memcpy(buffer + offset, &structReseau->score2, sizeof(int));
	offset += sizeof(int);
	
	//With a non-blocking socket i'll probably return false,
	//even if it works, so whatever... It's UDP anyway.
	server->send(buffer, sizePack);
}

void receiveGame(char *buff, Reseau* structReseau) //Reception des packets du Serveur vers Client
{
	int offset = 0;

	if ((*buff & 1) == 1)
	{
		structReseau->snd_paddle = true;
	}

	if ((*buff & 2) == 2)
	{
		structReseau->snd_goal = true;
	}

	if ((*buff & 4) == 4)
	{
		structReseau->snd_wall = true;
	}

	offset++;

	int x, y;

	for (int i = 0; i < 3; i++)
	{
		memcpy(&x, buff + offset, sizeof(int));
		offset += sizeof(int);
		memcpy(&y, buff + offset, sizeof(int));
		offset += sizeof(int);

		structReseau->colliders_kinematic[i].set_pos(x, y);
		
	}

	memcpy(&structReseau->score1, buff + offset, sizeof(int));
	offset += sizeof(int);

	memcpy(&structReseau->score2, buff + offset, sizeof(int));
	offset += sizeof(int);

	/*bool playing;
	packet >> playing;

	if (playing != true)//Si le Serveur nous envoie un message de déconnexion
	{
		std::cout << "Serveur déconnecté !\n";
		return true;
	}

	int x, y;

	packet >> x;
	packet >> y;

	structReseau->colliders_kinematic[0].set_pos(x, y);

	packet >> x;
	packet >> y;

	structReseau->colliders_kinematic[1].set_pos(x, y);

	packet >> x;
	packet >> y;

	structReseau->colliders_kinematic[2].set_pos(x, y);

	packet >> structReseau->score1 >> structReseau->score2;

	packet >> structReseau->snd_paddle >> structReseau->snd_goal >> structReseau->snd_wall;

	packet.clear();

	return false;*/
}

/*void sendInput(sf::Packet& packet, bool press, unsigned char key) //Ecriture du packet Client vers Serveur
{
	packet << true;
	packet << press << key;

}*/

bool receiveInput(sf::Packet& packet, struct Reseau* structReseau)//Reception des packets du Client vers Serveur
{
	bool playing;
	packet >> playing;

	if (playing != true)//Si le Client nous envoie un message de déconnexion
	{
		std::cout << "Client déconnecté !\n";
		return true;
	}

	bool press;
	unsigned char key;

	packet >> press >> key;

	structReseau->colliders_kinematic[1].keyboardEvents(key, press);

	packet.clear();

	return false;
}


void pollEventsServeur(Window& window, Kinematic& player) //Lecture des entrées sur le Serveur
{
	SDL_Event event;

	if (SDL_PollEvent(&event)) {
		player.pollEvents(event);
		window.pollEvents(event);
	}
}

void pollEventsClient(Window& window, struct Reseau structReseau, Client* client) //Lecture des entrées sur le Client
{
	SDL_Event event;

	if (SDL_PollEvent(&event)) {
		window.pollEvents(event);

		if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
		{
			bool press;

			if (event.type == SDL_KEYDOWN)
			{
				press = true;
			}
			else if (event.type == SDL_KEYUP)
			{
				press = false;
			}

			switch (event.key.keysym.sym)
			{
			case SDLK_LEFT:
				envoieClient(true, structReseau, client, 4, press);
				break;
			case SDLK_RIGHT:
				envoieClient(true, structReseau, client, 3, press);
				break;
			case SDLK_UP:
				envoieClient(true, structReseau, client, 1, press);
				break;
			case SDLK_DOWN:
				envoieClient(true, structReseau, client, 2, press);
				break;
			}

			std::cout << "Event sent" << endl;
		}
	}
}
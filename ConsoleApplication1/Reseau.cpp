#include "stdafx.h"
#include "Reseau.h"

void receptionTCP(struct Game* structGame, Client* client) //Recepetion avec le client
{
	char buffer[4] = "";

	while (strstr(buffer, "bye") == false && structGame->_exit == false)
	{
		client->reception(buffer, sizeof(buffer));
	}

	structGame->_exit = true;
}

void envoieClient(bool playing, struct Game* structGame, Client* client, unsigned char key, bool press)// Fonction d'envoie Client vers Serveur
{
	char* buffer;
	int sizePack;

	if (playing == true)
	{
		sizePack = 2;
		buffer = new char[sizePack];

		unsigned char a;

		if (press == true)
		{
			a = 1;
		}
		else {
			a = 0;
		}

		*(buffer) = a;
		*(buffer + 1) = key;

		client->envoie(buffer, sizePack);
	}
	else {
		sizePack = 3;
		buffer = new char[sizePack];

		std::memcpy(buffer, "bye", 3);

		client->envoie(buffer, sizePack);
	}



	if (client->envoie(buffer, sizePack) != true && structGame->_exit != true)//Si notre serveur c'est déja déjà déconnecter, on ne vérifie si le message a bien été envoyer
	{
		std::cout << "Erreur envoie \n";
	}

	delete[] buffer;

}

//TO BE CORRECTED
void sendGame(struct Game *structGame, ServerUDP* server)//Ecriture du packet Serveur vers Client
{
	int sizePack = sizeof(int) * 8 + 1;

	char buffer[sizeof(int) * 8 + 1];

	int offset = 0;

	unsigned char b = (structGame->snd_paddle << 0) + (structGame->snd_goal << 1) + (structGame->snd_wall << 2);

	memcpy(buffer, &b, sizeof(int));
	offset++;

	for (int i = 0; i < 3; i++)
	{
		int a = structGame->colliders_kinematic[i].get_x();
		memcpy(buffer + offset, &a, sizeof(int));
		offset += sizeof(int);

		a = structGame->colliders_kinematic[i].get_y();
		memcpy(buffer + offset, &a, sizeof(int));
		offset += sizeof(int);
	}

	memcpy(buffer + offset, &structGame->score1, sizeof(int));
	offset += sizeof(int);
	memcpy(buffer + offset, &structGame->score2, sizeof(int));
	offset += sizeof(int);
	
	//With a non-blocking socket i'll probably return false,
	//even if it works, so whatever... It's UDP anyway.
	server->send(buffer, sizePack);
}

void receiveGame(char *buff, Game* structGame) //Reception des packets du Serveur vers Client
{
	int offset = 0;

	if ((*buff & 1) == 1)
	{
		structGame->snd_paddle = true;
	}

	if ((*buff & 2) == 2)
	{
		structGame->snd_goal = true;
	}

	if ((*buff & 4) == 4)
	{
		structGame->snd_wall = true;
	}

	offset++;

	int x, y;

	for (int i = 0; i < 3; i++)
	{
		memcpy(&x, buff + offset, sizeof(int));
		offset += sizeof(int);
		memcpy(&y, buff + offset, sizeof(int));
		offset += sizeof(int);

		structGame->colliders_kinematic[i].set_pos(x, y);
		
	}

	memcpy(&structGame->score1, buff + offset, sizeof(int));
	offset += sizeof(int);

	memcpy(&structGame->score2, buff + offset, sizeof(int));
	offset += sizeof(int);
}

void pollEventsServeur(Window& window, Kinematic& player) //Lecture des entrées sur le Serveur
{
	SDL_Event event;

	if (SDL_PollEvent(&event)) {
		player.pollEvents(event);
		window.pollEvents(event);
	}
}

void pollEventsClient(Window& window, struct Game* structGame, Client* client) //Lecture des entrées sur le Client
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
				envoieClient(true, structGame, client, 4, press);
				break;
			case SDLK_RIGHT:
				envoieClient(true, structGame, client, 3, press);
				break;
			case SDLK_UP:
				if (structGame->up != press)
				{
					structGame->up = press;
					envoieClient(true, structGame, client, 1, press);
				}
				break;
			case SDLK_DOWN:
				if (structGame->down != press)
				{
					structGame->down = press;
					envoieClient(true, structGame, client, 2, press);
				}
				break;
			}

			//std::cout << "Event sent" << endl;
		}
	}
}
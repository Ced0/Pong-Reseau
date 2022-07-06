#include "stdafx.h"
#include "Reseau.h"
#include "Rect.h"
#include "Window.h"
#include "Text.h"
#include <time.h> 
#include "client.h"
#include "serveur.h"
#include "ServerUDP.h"
#include "ClientUDP.h"
//#include <SFML/System/Time.hpp>

#pragma comment(lib, "ws2_32.lib")

//#include "Input.h"

#define MAX_CLIENT 1

void gameLoopServeur(Reseau& structReseau, ServerUDP* servUDP);

void gameLoopClient(Reseau& structReseau, Client* client, ClientUDP* clientUDP);

void ball_reset(float& dx, float& dy, Kinematic& ball);

int main(int argc, char** argv)
{
	struct Reseau structReseau;

	std::string menu;


	std::cout << "Menu serveur/client : \n";
	std::cin >> menu;

	//sf::Time time = sf::seconds(float(420));



	if (menu == "client")
	{

		std::cout << "Entrez adresse IP serveur \n";//Connexion du client
		std::cin >> menu;

		//sf::Socket::Status status = structReseau.tcpSocket->connect(menu, 5300, time);
		Client client(menu.c_str(), 5300);

		//structReseau.udpSocket->bind(5300);

		std::cout << "Connexion\n";

		if (client.isConnected() == false)
		{
			std::cout << "Erreur connexion \n";
			return 1;

		}
		else {
			std::cout << "Connexion réussi \n";
		}

		ClientUDP clientUDP(menu.c_str(), 5301);

		Kinematic player(10, 100, 20, 250, 0, 0, 0, 255);//Création des variables/objets utlisié dans le thread réseau

		structReseau.colliders_kinematic.push_back(player);
		structReseau.colliders_kinematic.push_back(player);
		player.set_size(20, 20);
		structReseau.colliders_kinematic.push_back(player);

		std::thread threadReception(receptionTCP, &structReseau, &client);//Lancement thread Réseau et de la fonction principale du jeu

		gameLoopClient(structReseau, &client, &clientUDP);


		if (structReseau._exit == false)//Si le Client quitte, on envoie un message au serveur
		{
			
			structReseau._exit = true;
			threadReception.join();

			char buffer[] = "bye";

			client.envoie(buffer, sizeof(buffer));
			std::cout << "sent bye" << std::endl;
			//envoieClient(false, structReseau, 0, false);
		}

		if (threadReception.joinable())
		{
			threadReception.join();
		}

	}
	else if (menu == "serveur") {

		bool connecte = false;

		Kinematic player(10, 100, 20, 250, 0, 0, 0, 255);
		structReseau.colliders_kinematic.push_back(player);//Joueur local (serveur)


		player.set_pos(780, 250);
		structReseau.colliders_kinematic.push_back(player);//Joueur client


		player.set_pos(390, 290);
		player.set_size(20, 20);
		structReseau.colliders_kinematic.push_back(player);//Ball
		structReseau.maxClient = MAX_CLIENT;

		Serveur serv(&structReseau);
		serv.launchListenerThread(5300);

		ServerUDP servUDP(5301);

		//std::thread threadReception(clientServeur, &connecte, &structReseau);

		//std::cout << "Adresse Local : " << menu << std::endl;

		//std::cout << "Adresse Publique : " << menu << std::endl;

		while (serv.isConnected() == false)//En attandant d'etre connecté, broadcast ?, affichage IP
		{
			//Blabla

		}

		for (int i = 0; i < serv.getClientSize(); i++)
		{
			servUDP.addClient(serv.getClient(i));
		}

		//std::cout << structReseau.tcpSocket->getRemoteAddress().toString();
		gameLoopServeur(structReseau, &servUDP);

		structReseau._exit = true;

		//threadReception.join();

		/*if (structReseau.connecte == true)//Si le Client est toujours connecté, on lui dit que le Serveur se deconnecte
		{
			structReseau.connecte = false;
			envoieServeur(false, structReseau);
		}*/

	}
	else {

		std::cout << "Erreur menu";
	}

	//structReseau.udpSocket->unbind();
	//structReseau.tcpSocket->disconnect();

	return 0;
}

void gameLoopServeur(Reseau& structReseau, ServerUDP* servUDP)//Fonction Jeu Serveur
{
	srand(time(NULL));

	/////////////////////////////////////////////////////////////////////////////////Chargement des ressource du Jeu

	Window Window("Pong serveur", 800, 600);

	TTF_Font* police = TTF_OpenFont("res/arial.ttf", 30);//Police
	Window.set_police(police);

	if (!police)
	{
		std::cout << TTF_GetError() << std::endl;
		//system("pause");
		return;
	}

	Mix_Chunk* snd_pong_paddle = Mix_LoadWAV("res/ping_pong_8bit_beeep.wav");//Son
	//Window.set_sound(snd_pong_paddle);
	Mix_Chunk* snd_pong_goal = Mix_LoadWAV("res/ping_pong_8bit_peeeeeep.wav");
	//Window.set_sound(snd_pong_paddle);
	Mix_Chunk* snd_pong_wall = Mix_LoadWAV("res/ping_pong_8bit_plop.wav");
	//Window.set_sound(snd_pong_paddle);

	if (snd_pong_paddle == NULL || snd_pong_goal == NULL || snd_pong_wall == NULL)
	{
		return;
	}


	///////////////////////////////////////////////////////////////////////////////Création des variables/objets

	float dx;
	float dy;
	char collide_ball;

	unsigned int timer = 0;

	ball_reset(dx, dy, structReseau.colliders_kinematic[2]);

	Text text_score1(Window::renderer, "0", { 255, 0, 0, 255 }, police);
	Text text_score2(Window::renderer, "0", { 255, 0, 0, 255 }, police);

	std::thread threadEnvoie;

	while (!Window.isClosed() && structReseau.connecte == true)//Main loop
	{

		pollEventsServeur(Window, structReseau.colliders_kinematic[0]);//Input Joueur local

		if (timer + 42 < SDL_GetTicks())
		{

			timer = SDL_GetTicks();

			structReseau.snd_goal = false;
			structReseau.snd_paddle = false;
			structReseau.snd_wall = false;

			/////////////////////////////////////////////////////////////////////////////////////////////////////////Joueur

			for (unsigned char i = 0; i < 2; i++)
			{
				if (i == 1)
				{
					structReseau.mute.lock();
				}

				structReseau.colliders_kinematic[i].input_movement(structReseau.colliders_kinematic, i, false, true);

				if (structReseau.colliders_kinematic[i].get_y() < 0)
				{
					structReseau.colliders_kinematic[i].move(0, -structReseau.colliders_kinematic[i].get_y());
				}

				if (structReseau.colliders_kinematic[i].get_y2() > 600)
				{
					structReseau.colliders_kinematic[i].move(0, 600 - structReseau.colliders_kinematic[i].get_y2());
				}

				if (i == 1)
				{
					structReseau.mute.unlock();
				}
			}

			///////////////////////////////////////////////////////////////////////////////////////////////////Ball
			if (structReseau.colliders_kinematic[2].get_y() + dy < 0 || structReseau.colliders_kinematic[2].get_y2() + dy > 600) {
				dy = -dy;
				Mix_PlayChannel(-1, snd_pong_wall, 0);
				structReseau.snd_wall = true;

			}

			if (structReseau.colliders_kinematic[2].get_x() + dx < 0)
			{
				structReseau.score1++;
				text_score1.change_text(std::to_string(structReseau.score1), Window::renderer);
				ball_reset(dx, dy, structReseau.colliders_kinematic[2]);
				Mix_PlayChannel(-1, snd_pong_goal, 0);
				structReseau.snd_goal = true;


			}
			else if (structReseau.colliders_kinematic[2].get_x2() + dx > 800)
			{
				structReseau.score2++;
				text_score2.change_text(std::to_string(structReseau.score2), Window::renderer);
				ball_reset(dx, dy, structReseau.colliders_kinematic[2]);
				Mix_PlayChannel(-1, snd_pong_goal, 0);
				structReseau.snd_goal = true;
			}

			collide_ball = structReseau.colliders_kinematic[2].move(structReseau.colliders_kinematic, 2, dx, dy);

			switch (collide_ball) {
			case 'x':
				dx = -dx;
				break;
			case 'y':
				dy = -dy;
				break;
			case 2:
				dx = -dx;
				dy = -dy;
				break;
			}

			if (collide_ball != 0)
			{
				dx *= 1.1;
				dy *= 1.1;
				Mix_PlayChannel(-1, snd_pong_paddle, 0);
				structReseau.snd_paddle = true;
			}

			//////////////////////////////////////////////////////////////////////////////////////////Affichage



			Window.clear();

			for (unsigned char i = 0; i < structReseau.colliders_kinematic.size(); i++)
			{
				structReseau.colliders_kinematic[i].draw();
			}

			text_score1.display(760, 20, Window::renderer);
			text_score2.display(20, 20, Window::renderer);


			////////////////////////////////////////////////////////////////////////////////////////Envoie du jeu au Client
			/*if (threadEnvoie.joinable())
			{
				threadEnvoie.join();
			}*/

			//Socket is non-blocking, so I guess it won't take much time
			sendGame(&structReseau, servUDP);
			//threadEnvoie = std::thread(envoieServeur, true, structReseau);
		}

	}

	/////////////////////////////////////////////////////Fermeture

	//threadEnvoie.join();

	Mix_FreeChunk(snd_pong_wall);
	Mix_FreeChunk(snd_pong_paddle);
	Mix_FreeChunk(snd_pong_goal);

}




void ball_reset(float& dx, float& dy, Kinematic& ball)
{

	ball.set_pos(390, 290);
	int r;
	r = rand() % 2;
	if (r == 0)
	{
		r--;
	}
	dx = (rand() % 2 + 3) * r;

	r = rand() % 2;
	if (r == 0)
	{
		r--;
	}
	dy = (rand() % 2 + 3) * r;
}





void gameLoopClient(Reseau& structReseau, Client *client, ClientUDP *clientUDP)
{

	/////////////////////////////////////////////////////////////////////////////////Chargement des ressource du Jeu
	Window Window("Pong client", 800, 600);

	TTF_Font* police = TTF_OpenFont("res/arial.ttf", 30);//Police
	Window.set_police(police);

	if (!police)
	{
		std::cout << TTF_GetError() << std::endl;
		//system("pause");
		return;
	}

	Mix_Chunk* snd_pong_paddle = Mix_LoadWAV("res/ping_pong_8bit_beeep.wav");//Son
	//Window.set_sound(snd_pong_paddle);
	Mix_Chunk* snd_pong_goal = Mix_LoadWAV("res/ping_pong_8bit_peeeeeep.wav");
	//Window.set_sound(snd_pong_paddle);
	Mix_Chunk* snd_pong_wall = Mix_LoadWAV("res/ping_pong_8bit_plop.wav");
	//Window.set_sound(snd_pong_paddle);

	if (snd_pong_paddle == NULL || snd_pong_goal == NULL || snd_pong_wall == NULL)
	{
		return;
	}

	///////////////////////////////////////////////////////////////////////////////Création des variables/objets
	Text text_score1(Window::renderer, "0", { 255, 0, 0, 255 }, police);
	Text text_score2(Window::renderer, "0", { 255, 0, 0, 255 }, police);

	//unsigned int timer = 0;


	Window.clear();
	/////////////////////////////////////////////////////////////////////////////////////////////Boucle du jeu
	while (!Window.isClosed() && !structReseau._exit)//Main loop
	{

		pollEventsClient(Window, structReseau, client);

		char buff[sizeof(int) * 8 + 1];

		//if (timer + 42 < SDL_GetTicks())
		if(clientUDP->recv(buff, sizeof(int) * 8 + 1) == true)
		{
			receiveGame(buff, &structReseau);
			//timer = SDL_GetTicks();

			/////////////////////////////////////////////////////////////////Affichage
			Window.clear();

			for (unsigned char i = 0; i < structReseau.colliders_kinematic.size(); i++)
			{
				structReseau.colliders_kinematic[i].draw();
			}

			text_score1.change_text(std::to_string(structReseau.score1), Window::renderer);
			text_score2.change_text(std::to_string(structReseau.score2), Window::renderer);

			text_score1.display(760, 20, Window::renderer);
			text_score2.display(20, 20, Window::renderer);


			//////////////////////////////////////////////////////////////////Son
			if (structReseau.snd_goal)
				Mix_PlayChannel(-1, snd_pong_goal, 0);

			if (structReseau.snd_paddle)
				Mix_PlayChannel(-1, snd_pong_paddle, 0);

			if (structReseau.snd_wall)
				Mix_PlayChannel(-1, snd_pong_wall, 0);


			structReseau.snd_goal = false;
			structReseau.snd_paddle = false;
			structReseau.snd_wall = false;
		}

	}

	std::cout << "client closed" << endl;

	Mix_FreeChunk(snd_pong_wall);
	Mix_FreeChunk(snd_pong_paddle);
	Mix_FreeChunk(snd_pong_goal);
}

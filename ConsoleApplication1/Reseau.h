#pragma once
#include "stdafx.h"
#include <iostream>
#include <thread>
#include <SFML/Network.hpp>
#include <vector>
#include "Rect.h"
#include "client.h"
#include <mutex>
#include "ServerUDP.h"

struct Reseau {

	Reseau() { copie = false; }
	Reseau(const Reseau& copy) : _exit(copy._exit), colliders_kinematic(copy.colliders_kinematic), score1(copy.score1), score2(copy.score2), snd_paddle(copy.snd_paddle), snd_goal(copy.snd_goal), snd_wall(copy.snd_wall) { copie = true; }

	bool _exit = false;

	std::vector<Kinematic> colliders_kinematic;//Tableau des objet à afficher

	int score1 = 0;
	int score2 = 0;

	bool snd_paddle = false, snd_goal = false, snd_wall = false;

	bool copie;

	bool connecte = false;

	int maxClient;
	std::mutex mute;
};

void envoieClient(bool playing, struct Reseau structReseau, Client* client, unsigned char key = 0, bool press = false);//Envoie
void envoieServeur(bool playing, struct Reseau structReseau);

void clientServeur(bool* connecte, struct Reseau* structReseau);//Reception
void receptionTCP(struct Reseau* structReseau, Client* client);

void sendGame(struct Reseau* structReseau, ServerUDP* server);//Envoie et reception Serveur vers Client
void receiveGame(char* buff, struct  Reseau* structReseau);

//void sendInput(sf::Packet& packet, bool press, unsigned char key);//Envoie et reception Client vers Serveur
bool receiveInput(sf::Packet& packet, struct Reseau* structReseau);

void pollEventsServeur(Window& window, Kinematic& player);

void pollEventsClient(Window& window, struct Reseau structReseau, Client* client);
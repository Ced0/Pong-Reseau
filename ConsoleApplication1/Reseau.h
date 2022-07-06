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

struct Game {

	Game() { copie = false; }
	Game(const Game& copy) : _exit(copy._exit), colliders_kinematic(copy.colliders_kinematic), score1(copy.score1), score2(copy.score2), snd_paddle(copy.snd_paddle), snd_goal(copy.snd_goal), snd_wall(copy.snd_wall) { copie = true; }

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

void envoieClient(bool playing, struct Game structGame, Client* client, unsigned char key = 0, bool press = false);//Envoie

void receptionTCP(struct Game* structGame, Client* client);

void sendGame(struct Game* structGame, ServerUDP* server);//Envoie et reception Serveur vers Client
void receiveGame(char* buff, struct  Game* structGame);

void pollEventsServeur(Window& window, Kinematic& player);

void pollEventsClient(Window& window, struct Game structGame, Client* client);
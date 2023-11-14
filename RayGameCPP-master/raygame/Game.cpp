#include "Game.h"
#include <list>
#include <iostream>
using namespace std;

//Constructor
Game::Game()
{
	//Assign variables
	Game::objects;
}


void Game::Start()
{
	//Create objects
	//Player player;
	//Player* playerPtr = &player;
	objects.push_back(new Player()); //Create object

	objects.push_back(new GameObject("Thomas")); //Create object
}


void Game::Update()
{
	cout << objects.size() << endl;
	for (GameObject* obj : objects)
	{
		obj -> Update();
	}
}
#include "Game.h"
#include <list>
#include <iostream>
using namespace std;

//Constructor
Game::Game()
{
	//Assign variables
	Game::objects;
	

	//Create objects
	objects.push_back(new GameObject(9)); //Create object

	objects.push_back(new GameObject(8)); //Create object
	
}



void Game::Update()
{
	cout << objects.size() << endl;
	for (GameObject* obj : objects)
	{
		obj -> Update();
	}
}
#include "Game.h"
#include <list>
#include <iostream>
using namespace std;


Game* Game::instance = NULL;

//Constructor
Game::Game() { }

Game* Game::GetInstance()
{
	if (instance == NULL)
	{
		instance = new Game();
	}
	return instance;
}


void Game::Start()
{
	//Create objects
	scene.push_back(new Player());

	scene.push_back(new GameObject("Thomas"));
}


void Game::Update()
{

	cout << scene.size() << endl;
	for (GameObject* obj : scene)
	{
		obj -> Update();
	}
}

//Deconstructor
Game::~Game()
{
	scene.clear();
	instance = NULL;
}
#include "Game.h"
#include "raylib.h"
#include <list>
#include <iostream>
using namespace std;

//Singleton stuff
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
	//Load textures
	spr_player = LoadTexture("..//Assets//player.png");
	rect_player = Rectangle(); rect_player.x = 0; rect_player.y = 0; rect_player.width = spr_player.width; rect_player.height = spr_player.height;
	spr_bullet = LoadTexture("..//Assets//bullet.png");
	rect_bullet = Rectangle(); rect_bullet.x = 0; rect_bullet.y = 0; rect_bullet.width = spr_bullet.width; rect_bullet.height = spr_bullet.height;

	//Create objects
	InstanceObject(new Player());
	InstanceObject(new GameObject("Test Obj"));
}

void Game::Draw()
{

}


void Game::Update()
{
	Draw();

	cout << scene.size() << endl;
	for (int i = 0; i < scene.size(); i++)
	{
		scene[i]->Update();
	}
}

//Create a root object
void Game::InstanceObject(GameObject* newObj)
{
	cout << newObj << endl;
	scene.push_back(newObj);
	//Call the new object's start function
	newObj->Start();
}


//Deconstructor
Game::~Game()
{
	scene.clear();
	instance = NULL;
}
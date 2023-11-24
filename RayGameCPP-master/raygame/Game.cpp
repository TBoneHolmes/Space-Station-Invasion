#include "Game.h"
#include "raylib.h"
#include <list>
#include <iostream>
using namespace std;

//Singleton stuff
Game* Game::instance = NULL;

//Constructor
Game::Game()
{
	//Set camera/screen size
	cameraSize.x = 1280; cameraSize.y = 720;
}

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
	spr_background = LoadTexture("..//Assets//background.png");
	rect_background = Rectangle(); rect_background.x = 0; rect_background.y = 0; rect_background.width = spr_background.width; rect_background.height = spr_background.height;
	spr_player = LoadTexture("..//Assets//player.png");
	rect_player = Rectangle(); rect_player.x = 0; rect_player.y = 0; rect_player.width = spr_player.width; rect_player.height = spr_player.height;
	spr_bullet = LoadTexture("..//Assets//bullet.png");
	rect_bullet = Rectangle(); rect_bullet.x = 0; rect_bullet.y = 0; rect_bullet.width = spr_bullet.width; rect_bullet.height = spr_bullet.height;

	//Set game variables
	worldSize.x = 16 * spr_background.width; worldSize.y = 16 * spr_background.height;
	drawCollisions = true;
	//Set camera variables
	cameraPosition.x = 0; cameraPosition.y = 0;

	//Create objects
	InstanceObject(new Player(), worldSize.x / 2, worldSize.y / 2);
	InstanceObject(new EnemyDefault(), 400, 300);


	//ToggleFullscreen();

}

void Game::Draw()
{
	//Draw background
	Vector2 tileSize;
	tileSize.x = worldSize.x / spr_background.width;
	tileSize.y = worldSize.y / spr_background.height;
	for (int i = 0; i < tileSize.x * tileSize.y; i++)
	{
		DrawTexture(spr_background,
			(((i - (tileSize.x * ((int)floor((double)i / (double)tileSize.x)))) * spr_background.width) - cameraPosition.x),
			((((int)floor(i / tileSize.x)) * spr_background.height) - cameraPosition.y),
			WHITE);
	}
}


void Game::Update()
{
	Draw();

	//DEBUG
	cout << scene.size() << endl;

	//Update scene objects
	for (int i = 0; i < scene.size(); i++)
	{
		scene[i]->Update();
	}

	//Update camera position
	if (cameraOwner != nullptr)
	{
		cameraPosition.x = cameraOwner->globalPosition.x - (cameraSize.x / 2);
		cameraPosition.y = cameraOwner->globalPosition.y - (cameraSize.y / 2);
	}
	//Clamp camera position
	cameraPosition.x = Clamp(cameraPosition.x, 0, worldSize.x);
	cameraPosition.y = Clamp(cameraPosition.y, 0, worldSize.y);

	//DEBUG CAMERA MOVEMENT
	if (IsKeyDown(KEY_D))
	{
		cameraPosition.x += 1;
	}
	if (IsKeyDown(KEY_A))
	{
		cameraPosition.x -= 1;
	}
	if (IsKeyDown(KEY_W))
	{
		cameraPosition.y -= 1;
	}
	if (IsKeyDown(KEY_S))
	{
		cameraPosition.y += 1;
	}

}

//Create a root object
void Game::InstanceObject(GameObject* newObj, int posX, int posY)
{
	cout << newObj << endl; //DEBUG
	//Add object to scene list
	scene.push_back(newObj);
	//Set object position
	newObj->localPosition.x = posX;
	newObj->localPosition.y = posY;
	//Call the new object's start function
	newObj->Start();
}


//Deconstructor
Game::~Game()
{
	scene.clear();
	instance = NULL;
}
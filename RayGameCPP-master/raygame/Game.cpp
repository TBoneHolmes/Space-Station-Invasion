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
	spr_enemyDefault = LoadTexture("..//Assets//enemyDefault.png");
	rect_enemyDefault = Rectangle(); rect_enemyDefault.x = 0; rect_enemyDefault.y = 0; rect_enemyDefault.width = spr_enemyDefault.width; rect_enemyDefault.height = spr_enemyDefault.height;
	spr_explosion = LoadTexture("..//Assets//explosion_spritesheet.png");
	frames_explosion = 3; //The number of frames in this spritesheet
	rect_explosion = Rectangle(); rect_explosion.x = 0; rect_explosion.y = 0; rect_explosion.width = spr_explosion.width / frames_explosion; rect_explosion.height = spr_explosion.height;

	//Set game variables
	worldTileSize.x = 128; worldTileSize.y = 128;
	worldSize.x = worldTileSize.x * 32; worldSize.y = worldTileSize.y * 32;
	center.x = worldSize.x / 2; center.y = worldSize.y / 2;
	score = 0;
	drawCollisions = false;
	//Set player spawn pos
	playerSpawn.x = center.x; playerSpawn.y = center.y;
	//Set camera variables
	cameraPosition.x = 0; cameraPosition.y = 0;
	//Set enemy spawn timer
	enemySpawnTime = 8;
	enemySpawnTimer = enemySpawnTime;
	//Set player spawn time
	playerSpawnTime = 3;
	playerSpawnTimer = 0;

	//Create objects
	InstanceObject(new Minimap(), 0, 0);
	InstanceObject(new Player(), playerSpawn.x, playerSpawn.y);

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

	//Draw score
	int scoreXPos = 0;
	if (minimap != nullptr)
	{ scoreXPos = minimap->mapSize.x + 16; }
	DrawText("SCORE", scoreXPos, 8, 24, WHITE);
	string scoreString = "5387239572";
	DrawText(FormatText("%06i", score), scoreXPos, 32, 24, WHITE);
}


void Game::Update()
{
	Draw();
	ManageTimers();
	CameraPosition();

	//DEBUG
	//cout << scene.size() << endl;
	//cout << enemies.size() << endl;

	//Update scene objects
	for (int i = 0; i < scene.size(); i++)
	{
		scene[i]->Update();
	}

}

void Game::ManageTimers()
{
	//Enemy spawn tick down
	if (enemySpawnTimer > 0)
	{
		enemySpawnTimer -= GetFrameTime();

		//Timeout
		if (enemySpawnTimer <= 0)
		{
			enemySpawnTimer = enemySpawnTimer = enemySpawnTime;
			SpawnEnemy();
		}
	}

	//Player spawn tick down
	if (playerSpawnTimer > 0)
	{
		playerSpawnTimer -= GetFrameTime();

		//Timeout
		if (playerSpawnTimer <= 0)
		{
			playerSpawnTimer = 0;
			SpawnPlayer();
		}
	}
}

void Game::CameraPosition()
{
	//Update camera position
	if (cameraOwner != nullptr)
	{
		cameraPosition.x = cameraOwner->globalPosition.x - (cameraSize.x / 2);
		cameraPosition.y = cameraOwner->globalPosition.y - (cameraSize.y / 2);
	}
	//Clamp camera position
	cameraPosition.x = Clamp(cameraPosition.x, 0, worldSize.x - (cameraSize.x));
	cameraPosition.y = Clamp(cameraPosition.y, 0, worldSize.y - (cameraSize.y));
}

void Game::SpawnEnemy()
{
	//Choose enemy spawn pos
	//0 = top, 1 = right, 2 = bottom, 3 = left
	int edgeChoice = GetRandomValue(0, 3);
	float edgeDist;
	Vector2 spawnPos;
	switch (edgeChoice)
	{
		case 0:
			edgeDist = GetRandomValue(0, worldSize.x);
			spawnPos.x = edgeDist;
			spawnPos.y = -32;
			cout << "CASE 0 :: " << spawnPos.x << endl;
			break;
		case 1:
			edgeDist = GetRandomValue(0, worldSize.y);
			spawnPos.x = worldSize.x + 32;
			spawnPos.y = edgeDist;
			cout << "CASE 1 :: " << spawnPos.x << endl;
			break;

		case 2:
			edgeDist = GetRandomValue(0, worldSize.x);
			spawnPos.x = edgeDist;
			spawnPos.y = worldSize.y + 32;
			cout << "CASE 2 :: " << spawnPos.x << endl;
			break;
		case 3:
			edgeDist = GetRandomValue(0, worldSize.y);
			spawnPos.x = -32;
			spawnPos.y = edgeDist;
			cout << "CASE 3 :: " << spawnPos.x << endl;
			break;

	}
	//Create enemy
	InstanceObject(new EnemyDefault, spawnPos.x, spawnPos.y);
	//cout << "Enemy spawned" << endl;
}

void Game::SpawnPlayer()
{
	InstanceObject(new Player, playerSpawn.x, playerSpawn.y);
}

//Create a root object
void Game::InstanceObject(GameObject* newObj, int posX, int posY)
{
	//cout << newObj << endl; //DEBUG
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
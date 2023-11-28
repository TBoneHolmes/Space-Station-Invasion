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
	spr_base = LoadTexture("..//Assets//base.png");
	rect_base = Rectangle(); rect_base.x = 0; rect_base.y = 0; rect_base.width = spr_base.width; rect_base.height = spr_base.height;


	//Set game variables
	worldTileSize.x = 128; worldTileSize.y = 128;
	worldSize.x = worldTileSize.x * 32; worldSize.y = worldTileSize.y * 32;
	center.x = worldSize.x / 2; center.y = worldSize.y / 2;
	score = 0;
	drawCollisions = true;
	//Set player spawn pos
	playerSpawn.x = center.x; playerSpawn.y = center.y + 128;
	//Set camera variables
	cameraPosition.x = 0; cameraPosition.y = 0;
	//Set player spawn time
	playerSpawnTime = 3;
	playerSpawnTimer = 0;

	gameover = false;

	//Create objects
	StartGame();

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

	//Draw gameover
	if (gameover)
	{
		int textWidth = MeasureText("GAME OVER", 96);
		DrawText("GAME OVER", (cameraSize.x / 2) - (textWidth / 2), cameraSize.y / 2 - 256, 96, RED);
		textWidth = MeasureText("YOUR BASE WAS DESTROYED", 32);
		DrawText("YOUR BASE WAS DESTROYED", (cameraSize.x / 2) - (textWidth / 2), cameraSize.y / 2 - 160, 32, WHITE);
		textWidth = MeasureText(FormatText("SCORE: %06i", score), 32);
		DrawText(FormatText("SCORE: %06i", score), (cameraSize.x / 2) - (textWidth / 2), cameraSize.y / 2 - 32, 32, GREEN);
	}
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
	//Draw other objects
	for (int draw = 0; draw < 5; draw++)
	{
		for (int i = 0; i < scene.size(); i++)
		{
			if (scene[i]->drawOrder == draw)
			{
				scene[i]->Draw();
			}
		}
	}

}

void Game::ManageTimers()
{
	//Enemy spawn tick down
	if (enemySpawnTimer > 0)
	{
		enemySpawnTimer -= GetFrameTime();

		//Timeout
		if (enemySpawnTimer <= 0 && !gameover)
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

void Game::Gameover()
{
	gameover = true;

	//Destroy everything
	for (GameObject* obj : scene)
	{
		obj->~GameObject();
	}

	//Set references to null
	player = nullptr;
	base = nullptr;

	//Stop timers
	playerSpawnTimer = 0;
	enemySpawnTimer = 0;

	//Create restart button
	InstanceObject(new Button("RESTART", 160, 48), cameraSize.x / 2, (cameraSize.y / 2) + 64);
	//Create quit button
	InstanceObject(new Button("QUIT", 160, 48), cameraSize.x / 2, (cameraSize.y / 2) + 128);
}

void Game::StartGame()
{
	gameover = false;
	score = 0;

	//Instance objects
	InstanceObject(new Base(), center.x, center.y);
	InstanceObject(new Minimap(), 0, 0);
	InstanceObject(new Player(), playerSpawn.x, playerSpawn.y);

	SpawnEnemy();

	//Start enemy spawn timer
	enemySpawnTime = 8;
	enemySpawnTimer = enemySpawnTime;

	//Destroy buttons
	for (GameObject* obj : scene)
	{
		if (obj->name == "Button")
		{cout << obj->name << "  deleted" << endl;
			obj->~GameObject(); }
	}
}


//Deconstructor
Game::~Game()
{
	scene.clear();
	instance = NULL;
}
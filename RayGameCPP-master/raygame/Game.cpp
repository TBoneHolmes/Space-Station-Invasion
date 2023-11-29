#include "Game.h"
#include "raylib.h"
#include <list>
#include <iostream>
#include <algorithm>
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
	spr_asteroidS = LoadTexture("..//Assets//asteroid_s.png");
	rect_asteroidS = Rectangle(); rect_asteroidS.x = 0; rect_asteroidS.y = 0; rect_asteroidS.width = spr_asteroidS.width; rect_asteroidS.height = spr_asteroidS.height;
	spr_asteroidM = LoadTexture("..//Assets//asteroid_m.png");
	rect_asteroidM = Rectangle(); rect_asteroidM.x = 0; rect_asteroidM.y = 0; rect_asteroidM.width = spr_asteroidM.width; rect_asteroidM.height = spr_asteroidM.height;
	spr_asteroidL = LoadTexture("..//Assets//asteroid_l.png");
	rect_asteroidL = Rectangle(); rect_asteroidL.x = 0; rect_asteroidL.y = 0; rect_asteroidL.width = spr_asteroidL.width; rect_asteroidL.height = spr_asteroidL.height;


	//Set game variables
	worldTileSize.x = 128; worldTileSize.y = 128;
	worldSize.x = worldTileSize.x * 32; worldSize.y = worldTileSize.y * 32;
	center.x = worldSize.x / 2; center.y = worldSize.y / 2;
	score = 0;
	drawCollisions = false;
	//Set player spawn pos
	playerSpawn.x = center.x; playerSpawn.y = center.y + 128;
	//Set camera variables
	cameraPosition.x = 0; cameraPosition.y = 0;
	//Set player spawn time
	playerSpawnTime = 3;
	playerSpawnTimer = 0;
	//Set chunks
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			//Set chunk size
			chunk[row * 4 + col].width = worldSize.x / 4;
			chunk[row * 4 + col].height = worldSize.y / 4;
			//Set chunk location
			chunk[row * 4 + col].x = (worldSize.x / 4) * row;
			chunk[row * 4 + col].y = (worldSize.y / 4) * col;
		}
	}


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
	//cout <<"Wave " << wave << " : " << enemySpawnTimer << endl;

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
	
	//Manage waves
	enemiesToSpawn -= 1;
	if (enemiesToSpawn <= 0)
	{
		WaveIncrease();
	}
}

void Game::SpawnPlayer()
{
	InstanceObject(new Player, playerSpawn.x, playerSpawn.y);
}

void Game::SpawnAsteroid()
{
	//Look for a free chunk to spawn the asteroid in
	Rectangle freeChunk; freeChunk.x = -1;

	//Shuffle the order of chunks to search in (and exclude the 4 middle chunks)
	int newOrder[12] = {0, 1, 2, 3, 4, 5, 8, 9, 12, 13, 14, 15};
	random_shuffle(begin(newOrder), end(newOrder));
	
	//if (asteroids.size() > 0)
	//{
		//Search each chunk until a free one is found
		for (int i = 0; i < sizeof(chunk) / sizeof(Rectangle); i++)
		{
			if (i != 6 && i != 7 && i != 10 && i != 11)
			{
				Vector2 newSpawnPlus;
				newSpawnPlus.x = GetRandomValue(128, chunk[i].width - 128);
				newSpawnPlus.y = GetRandomValue(128, chunk[i].height - 128);
				//Spawn asteroid
				InstanceObject(new Asteroid(), (chunk[i].x + newSpawnPlus.x), (chunk[i].y + newSpawnPlus.y));
			}
			//for (GameObject* asteroid : asteroids)
			//{
			//	if	(//Make sure this chunk is free of any other asteroids
			//		(asteroid->globalPosition.x < chunk[newOrder[i]].x || asteroid->globalPosition.x > chunk[newOrder[i]].x + chunk[newOrder[i]].width)
			//		&& (asteroid->globalPosition.y < chunk[newOrder[i]].y || asteroid->globalPosition.y > chunk[newOrder[i]].y + chunk[newOrder[i]].height))
			//	{
			//		freeChunk = chunk[newOrder[i]];
			//		break;
			//	}
			//}
			//if (freeChunk.x != -1)
			//{
			//	break;
			//}
		}
	//}
	//No asteroids currently exist. Just choose the first chunk option
	//else
	//{
	//	freeChunk = chunk[newOrder[0]];
	//	//cout << "Chunk chosen: " << newOrder[0] << endl;
	//}
	
	//A free chunk was found. Spawn asteroid in the chunk
	//if (freeChunk.x != -1)
	//{
	//	//Choose random position within the chunk
	//	Vector2 newSpawnPlus;
	//	newSpawnPlus.x = GetRandomValue(128, freeChunk.width - 128);
	//	newSpawnPlus.y = GetRandomValue(128, freeChunk.height - 128);
	//	//Spawn asteroid
	//	InstanceObject(new Asteroid(), (freeChunk.x + newSpawnPlus.x), (freeChunk.y + newSpawnPlus.y));
	//	//cout << "Spawned asteroid at: " << freeChunk.x + newSpawnPlus.x << ", " << freeChunk.y + newSpawnPlus.y << endl;
	//}
	//else
	//{
	//	cout << "WHAT";
	//}
}


void Game::WaveIncrease()
{
	wave += 1;
	//Reset spawn count
	enemiesToSpawn = 6;
	//Reset spawn timer
	if (enemySpawnTime > 1)
	{ enemySpawnTime -= 1; }
	enemySpawnTimer = enemySpawnTime;
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
	//Reset game variables
	gameover = false;
	score = 0;
	//Reset wave
	wave = 1;
	enemiesToSpawn = 6;

	//Destroy all objects
	for (GameObject* obj : scene)
	{
		obj->~GameObject();
	}

	//Instance objects
	InstanceObject(new Base(), center.x, center.y);
	InstanceObject(new Minimap(), 0, 0);
	InstanceObject(new Player(), playerSpawn.x, playerSpawn.y);
	//Create asteroids
	SpawnAsteroid();

	//Start enemy spawn timer
	enemySpawnTime = 8;
	enemySpawnTimer = enemySpawnTime;

}


//Deconstructor
Game::~Game()
{
	scene.clear();
	instance = NULL;
}
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
	isGameRunning = true;
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
	spr_title = LoadTexture("..//Assets//Sprites//title.png");
	spr_background = LoadTexture("..//Assets//Sprites//background.png");
	rect_background = Rectangle(); rect_background.x = 0; rect_background.y = 0; rect_background.width = spr_background.width; rect_background.height = spr_background.height;
	spr_player = LoadTexture("..//Assets//Sprites//player.png");
	rect_player = Rectangle(); rect_player.x = 0; rect_player.y = 0; rect_player.width = spr_player.width; rect_player.height = spr_player.height;
	spr_playerBooster = LoadTexture("..//Assets//Sprites//player_booster.png");
	frames_playerBooster = 2; //The number of frames in this spritesheet
	rect_playerBooster = Rectangle(); rect_playerBooster.x = 0; rect_playerBooster.y = 0; rect_playerBooster.width = spr_playerBooster.width / frames_playerBooster; rect_playerBooster.height = spr_playerBooster.height;
	spr_bullet = LoadTexture("..//Assets//Sprites//bullet.png");
	rect_bullet = Rectangle(); rect_bullet.x = 0; rect_bullet.y = 0; rect_bullet.width = spr_bullet.width; rect_bullet.height = spr_bullet.height;
	spr_enemyDefault = LoadTexture("..//Assets//Sprites//enemyDefault.png");
	rect_enemyDefault = Rectangle(); rect_enemyDefault.x = 0; rect_enemyDefault.y = 0; rect_enemyDefault.width = spr_enemyDefault.width; rect_enemyDefault.height = spr_enemyDefault.height;
	spr_boss = LoadTexture("..//Assets//Sprites//boss.png");
	rect_boss = Rectangle(); rect_boss.x = 0; rect_boss.y = 0; rect_boss.width = spr_boss.width; rect_boss.height = spr_boss.height;
	spr_bossShield = LoadTexture("..//Assets//Sprites//bossShield.png");
	rect_bossShield = Rectangle(); rect_bossShield.x = 0; rect_bossShield.y = 0; rect_bossShield.width = spr_bossShield.width; rect_bossShield.height = spr_bossShield.height;
	spr_explosion = LoadTexture("..//Assets//Sprites//explosion_spritesheet.png");
	frames_explosion = 3; //The number of frames in this spritesheet
	rect_explosion = Rectangle(); rect_explosion.x = 0; rect_explosion.y = 0; rect_explosion.width = spr_explosion.width / frames_explosion; rect_explosion.height = spr_explosion.height;
	spr_base = LoadTexture("..//Assets//Sprites//base.png");
	rect_base = Rectangle(); rect_base.x = 0; rect_base.y = 0; rect_base.width = spr_base.width; rect_base.height = spr_base.height;
	spr_asteroidS = LoadTexture("..//Assets//Sprites//asteroid_s.png");
	rect_asteroidS = Rectangle(); rect_asteroidS.x = 0; rect_asteroidS.y = 0; rect_asteroidS.width = spr_asteroidS.width; rect_asteroidS.height = spr_asteroidS.height;
	spr_asteroidM = LoadTexture("..//Assets//Sprites//asteroid_m.png");
	rect_asteroidM = Rectangle(); rect_asteroidM.x = 0; rect_asteroidM.y = 0; rect_asteroidM.width = spr_asteroidM.width; rect_asteroidM.height = spr_asteroidM.height;
	spr_asteroidL = LoadTexture("..//Assets//Sprites//asteroid_l.png");
	rect_asteroidL = Rectangle(); rect_asteroidL.x = 0; rect_asteroidL.y = 0; rect_asteroidL.width = spr_asteroidL.width; rect_asteroidL.height = spr_asteroidL.height;
	spr_powerup = LoadTexture("..//Assets//Sprites//powerup.png");
	rect_powerup = Rectangle(); rect_powerup.x = 0; rect_powerup.y = 0; rect_powerup.width = spr_powerup.width; rect_powerup.height = spr_powerup.height;

	//Load sounds
	sfx_boostPlayer = LoadSound("..//Assets//Sounds//boost_player.wav");
	sfx_shootPlayer = LoadSound("..//Assets//Sounds//shoot_player.wav");
	SetSoundVolume(sfx_shootPlayer, 0.5);
	sfx_shootEnemy = LoadSound("..//Assets//Sounds//shoot_enemy.wav");
	SetSoundVolume(sfx_shootEnemy, 0.5);
	sfx_hitPlayer = LoadSound("..//Assets//Sounds//hit_player.wav");
	SetSoundVolume(sfx_hitPlayer, 0.7);
	sfx_hitEnemy = LoadSound("..//Assets//Sounds//hit_enemy.wav");
	SetSoundVolume(sfx_hitEnemy, 0.7);
	sfx_hitAsteroid = LoadSound("..//Assets//Sounds//hit_asteroid.wav");
	SetSoundVolume(sfx_hitAsteroid, 0.7);
	sfx_hitBase = LoadSound("..//Assets//Sounds//hit_base.wav");
	sfx_explodePlayer = LoadSound("..//Assets//Sounds//explode_player.wav");
	sfx_explodeEnemy = LoadSound("..//Assets//Sounds//explode_enemy.wav");
	sfx_explodeAsteroid = LoadSound("..//Assets//Sounds//explode_asteroid.wav");
	sfx_buttonHover = LoadSound("..//Assets//Sounds//buttonHover.wav");
	SetSoundVolume(sfx_buttonHover, 0.7);
	sfx_buttonClick = LoadSound("..//Assets//Sounds//buttonClick.wav");
	SetSoundVolume(sfx_buttonClick, 0.7);
	sfx_pause = LoadSound("..//Assets//Sounds//pause.wav");
	SetSoundVolume(sfx_pause, 0.5);
	sfx_unpause = LoadSound("..//Assets//Sounds//unpause.wav");
	SetSoundVolume(sfx_unpause, 0.5);
	sfx_gameover = LoadSound("..//Assets//Sounds//gameover.wav");
	sfx_powerup = LoadSound("..//Assets//Sounds//powerup.wav");
	SetSoundVolume(sfx_powerup, 0.5);
	sfx_bossApproach = LoadSound("..//Assets//Sounds//bossApproach.wav");
	SetSoundVolume(sfx_bossApproach, 0.5);

	//Load font
	fnt_gameover = LoadFont("..//Assets//Fonts//gameover.ttf");

	//Set game icon
	icon = LoadImage("..//Assets//title.png");
	SetWindowIcon(icon);



	//Set game variables
	worldTileSize.x = 128; worldTileSize.y = 128;
	worldSize.x = worldTileSize.x * 32; worldSize.y = worldTileSize.y * 32;
	center.x = worldSize.x / 2; center.y = worldSize.y / 2;
	score = 0;
	killCount = 0;
	drawCollisions = false; //For debugging; Change to 'true' to draw collision shapes
	//Player spawn pos
	playerSpawn.x = center.x; playerSpawn.y = center.y + 128;
	//Camera position
	cameraPosition.x = 0; cameraPosition.y = 0;
	//Player spawn time
	playerSpawnTime = 3;
	playerSpawnTimer = 0;
	//Set instruction time
	instructionTime = 12;
	//Boss text timer
	bossTextTimer = 0;
	//Set up waves
	wave = 1;
	bossWave = 4; //If the wave is divisible by bossWave, a boss will appear
	bossSpawned = false; //A boss has been spawned for the current wave
	//Set chunks
	for (int col = 0; col < 4; col++)
	{
		for (int row = 0; row < 4; row++)
		{
			//Set chunk size
			chunk[col + row * 4].width = worldSize.x / 4;
			chunk[col + row * 4].height = worldSize.y / 4;
			//Set chunk location
			chunk[col + row * 4].x = (worldSize.x / 4) * col;
			chunk[col + row * 4].y = (worldSize.y / 4) * row;
		}
	}

	//Powerup stuff
	powerupSpawnRange.x = 6; powerupSpawnRange.y = 12;

	menuOpen = true;
	gameover = false;
	gamePaused = false;
	freeze = 0;

	StartMenu();

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

	//Draw menu
	if (menuOpen)
	{
		//Draw title
		DrawTexture(spr_title, (cameraSize.x / 2) - (spr_title.width / 2), (cameraSize.y / 2) - (spr_title.height / 2) - 160, WHITE);
		//Draw fullscreen shortcut
		char* text = (char*)"(F11)";
		int textWidth = MeasureText(text, 24);
		DrawText(text, (cameraSize.x - 96) - (textWidth / 2), cameraSize.y - 120, 24, WHITE);
	}

	//Draw gameover
	else if (gameover)
	{
		char* text = (char*)"GAME OVER";
		int textWidth = MeasureText(text, 96);
		Vector2 textPos; textPos.x = (cameraSize.x / 2) - (textWidth / 2) - 144; textPos.y = cameraSize.y / 2 - 256;
		DrawTextEx(fnt_gameover, text, textPos, 96, 0, RED);
		text = (char*)"YOUR BASE WAS DESTROYED";
		textWidth = MeasureText(text, 32);
		DrawText(text, (cameraSize.x / 2) - (textWidth / 2), cameraSize.y / 2 - 152, 32, WHITE);
		text = (char*)"SCORE: %06i";
		textWidth = MeasureText(TextFormat(text, score), 32);
		DrawText(TextFormat(text, score), (cameraSize.x / 2) - (textWidth / 2), cameraSize.y / 2 - 32, 32, GREEN);
		text = (char*)"KILLS: % 00i";
		textWidth = MeasureText(TextFormat(text, killCount), 32);
		DrawText(TextFormat(text, killCount), (cameraSize.x / 2) - (textWidth / 2) - 45, cameraSize.y / 2, 32, GREEN);
	}

	//Draw pause
	else if (gamePaused)
	{
		char* text = (char*)"PAUSED";
		int textWidth = MeasureText(text, 64);
		DrawText(text, (cameraSize.x / 2) - (textWidth / 2), cameraSize.y / 2 - 128, 64, WHITE);
		text = (char*)"PRESS 'ENTER' TO CONTINUE";
		textWidth = MeasureText(text, 32);
		DrawText(text, (cameraSize.x / 2) - (textWidth / 2), cameraSize.y / 2, 32, WHITE);
	}


	//DEBUG Draw chunk grid
	//for (int i = 0; i < sizeof(chunk) / sizeof(int); i++)
	//{
	//	DrawLine(chunk[i].x - cameraPosition.x, chunk[i].y - cameraPosition.y, chunk[i].x + chunk[i].width - cameraPosition.x, chunk[i].y - cameraPosition.y, GREEN);
	//	DrawLine(chunk[i].x + chunk[i].width - cameraPosition.x, chunk[i].y - cameraPosition.y, chunk[i].x + chunk[i].width - cameraPosition.x, chunk[i].y + chunk[i].height - cameraPosition.y, GREEN);
	//	DrawLine(chunk[i].x + chunk[i].width - cameraPosition.x, chunk[i].y + chunk[i].height - cameraPosition.y, chunk[i].x - cameraPosition.x, chunk[i].y + chunk[i].height - cameraPosition.y, GREEN);
	//	DrawLine(chunk[i].x - cameraPosition.x, chunk[i].y + chunk[i].height - cameraPosition.y, chunk[i].x - cameraPosition.x, chunk[i].y - cameraPosition.y, GREEN);
	//	DrawText(FormatText("%00i", i), chunk[i].x + 32 - cameraPosition.x, chunk[i].y + 32 - cameraPosition.y, 32, GREEN);
	//}
}


void Game::Update()
{
	//DEBUG
	//cout << scene.size() << endl;
	//cout << enemies.size() << endl;
	//cout << wave << endl;

	Draw();

	if (!gamePaused)
	{
		ManageTimers();
		CameraPosition();
	}
	Inputs();

	//Update scene objects
	if (!freeze)
	{
		for (int i = 0; i < scene.size(); i++)
		{
			if ((!gamePaused || scene[i]->pauseIgnore)
				&& (!gameover || scene[i]->gameoverIgnore))
			{ 
				scene[i]->Update();
			}
		}
	}
	//Draw other objects
	for (int draw = 0; draw < 6; draw++)
	{
		for (int i = 0; i < scene.size(); i++)
		{
			if (scene[i]->drawOrder == draw
				&& (!gamePaused || scene[i]->pauseIgnore)
				&& (!gameover || scene[i]->gameoverIgnore))
			{
				scene[i]->Draw();
			}
		}
	}

	ClearGarbageCollection();

	//Clamp score
	score = Clamp(score, 0, 999999);
	killCount = Clamp(killCount, 0, 999);
	
	
	//Manage boss spawning
	if (wave % bossWave == 0 && enemies.size() == 0 && !bossSpawned)
	{
		SpawnEnemy(true);
	}
}

void Game::ManageTimers()
{
	cout << "Wave " << wave << " | Spawn timer: " << enemySpawnTime << " | Per spawn: " << enemiesPerSpawn << endl;

	//Enemy spawn tick down
	if (enemySpawnTimer > 0 && wave % bossWave != 0)
	{
		enemySpawnTimer -= GetFrameTime();

		//Timeout
		if (enemySpawnTimer <= 0 && !gameover)
		{
			enemySpawnTimer = enemySpawnTime;
			if (wave % bossWave != 0)
			{
				cout << wave << endl;
				//Spawn enemies
				for (int i = 0; i < enemiesPerSpawn; i++)
				{ SpawnEnemy(false); }
				//Manage wave increase
				enemiesToSpawn -= 1;
				if (wave % bossWave != 0)
				{
					if (enemiesToSpawn <= 0)
					{
						WaveIncrease();
					}
				}
			}
			
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

	//Start timer
	if (instructionTimer > 0)
	{
		instructionTimer -= GetFrameTime();

		//Timeout
		if (instructionTimer <= 0)
		{
			instructionTimer = 0;
			//Set enemy spawn timer and spawn the first enemy
			enemySpawnTimer = enemySpawnTime;
			enemiesToSpawn -= 1;
			SpawnEnemy(false);
		}
	}

	//Boss text timer
	if (bossTextTimer > 0)
	{
		bossTextTimer -= GetFrameTime();

		//Timeout
		if (bossTextTimer <= 0)
		{ bossTextTimer = 0; }
	}

	//Game freeze
	if (freeze > 0)
	{
		freeze -= GetFrameTime();

		//Timeout
		if (freeze <= 0)
		{
			freeze = 0;
		}
	}
}

void Game::CameraPosition()
{
	//Screen shake
	if (screenshake > 0.1)
	{
		screenshake = Lerp(screenshake, 0, 0.1);
		cameraShakePos.x = GetRandomValue(-screenshake, screenshake);
		cameraShakePos.y = GetRandomValue(-screenshake, screenshake);
	}
	else
	{
		screenshake = 0;
		cameraShakePos = Vector2Zero();
	}

	//Update camera position
	if (cameraOwner != nullptr)
	{
		cameraPosition.x = cameraOwner->globalPosition.x - (cameraSize.x / 2);
		cameraPosition.y = cameraOwner->globalPosition.y - (cameraSize.y / 2);
	}
	//Add screenshake
	cameraPosition = Vector2Add(cameraPosition, cameraShakePos);
	//Clamp camera position
	cameraPosition.x = Clamp(cameraPosition.x, 0, worldSize.x - (cameraSize.x));
	cameraPosition.y = Clamp(cameraPosition.y, 0, worldSize.y - (cameraSize.y));
}

void Game::Inputs()
{
	//Toggle pause
	if (IsKeyPressed(KEY_ENTER) && !gameover && !menuOpen)
	{
		gamePaused = !gamePaused;
		//Play sound
		if (gamePaused)
		{
			PlaySound(sfx_pause);
		}
		else
		{
			PlaySound(sfx_unpause);
		}
	}
	//Toggle fullscren key
	if (IsKeyPressed(KEY_F11))
	{
		PlaySound(sfx_buttonClick);
		ToggleFullscreen();
	}

	//Clamp mouse position
	if (!gamePaused && !menuOpen && !gameover)
	{
		SetMousePosition(
		Clamp(GetMousePosition().x, 0, cameraSize.x),
		Clamp(GetMousePosition().y, 0, cameraSize.y));
	}
}


void Game::ClearGarbageCollection()
{
	//Garbage collection
	if (garbageCollection.size() > 0)
	{
		for (int i = 0; i < garbageCollection.size(); i++)
		{
			delete garbageCollection[i];
		}
		garbageCollection.clear();
	}
}













void Game::SpawnEnemy(bool boss)
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
			break;
		case 1:
			edgeDist = GetRandomValue(0, worldSize.y);
			spawnPos.x = worldSize.x + 32;
			spawnPos.y = edgeDist;
			break;

		case 2:
			edgeDist = GetRandomValue(0, worldSize.x);
			spawnPos.x = edgeDist;
			spawnPos.y = worldSize.y + 32;
			break;
		case 3:
			edgeDist = GetRandomValue(0, worldSize.y);
			spawnPos.x = -32;
			spawnPos.y = edgeDist;
			break;

	}
	//Create enemy
	if (!boss)
	{
		InstanceObject(new EnemyDefault(), spawnPos.x, spawnPos.y);
	}
	else
	{
		InstanceObject(new Boss(), spawnPos.x, spawnPos.y);
		bossSpawned = true;
		bossTextTimer = 1.8;
		PlaySound(sfx_bossApproach);
	}
}

void Game::SpawnPlayer()
{
	InstanceObject(new Player, playerSpawn.x, playerSpawn.y);
}

//Spawns an asteroid at a given chunk
void Game::SpawnAsteroid(int spawnChunk)
{
	Vector2 newSpawnPlus;
	do { //Set a random spawn position and ensure it's not in camerea view
		newSpawnPlus.x = GetRandomValue(128, chunk[spawnChunk].width - 128);
		newSpawnPlus.y = GetRandomValue(128, chunk[spawnChunk].height - 128);
	} while (chunk[spawnChunk].x + newSpawnPlus.x > cameraPosition.x && chunk[spawnChunk].x + newSpawnPlus.x < cameraPosition.x + cameraSize.x
		&& chunk[spawnChunk].y + newSpawnPlus.y > cameraPosition.y && chunk[spawnChunk].y + newSpawnPlus.y < cameraPosition.y + cameraSize.y);
	//Spawn asteroid
	InstanceObject(new Asteroid(), (chunk[spawnChunk].x + newSpawnPlus.x), (chunk[spawnChunk].y + newSpawnPlus.y));
}

//Spawns asteroids based on the wave count
void Game::SpawnWaveAsteroids()
{
	int newAsteroidCount = 0;
	newAsteroidCount = Clamp(newAsteroidCount, 10 + (2 * wave), 36); //The number of asteroids to exist at the start of the next round
	//Set a random order of the chunks to check through for spawning
	int newOrder[12] = { 0, 1, 2, 3, 4, 7, 8, 11, 12, 13, 14, 15 };
	random_shuffle(begin(newOrder), end(newOrder));
	//Spawn asteroids
	int i = 0;
	while (CountLargeAsteroids() < newAsteroidCount)
	{
		//Reshuffle order
		if (i / 12 >= 1)
		{
			random_shuffle(begin(newOrder), end(newOrder));
		}

		//Spawn an asteroid in the newOrder[i] chunk if the chunk isn't cluttered with too many asteroids
		if (CheckAsteroidChunk(newOrder[i - (12 * (int)floorf(((float)i / (float)12)))]) <= (int)floorf((float)i / (float)12))
		{
			SpawnAsteroid(newOrder[i - (12 * (int)floorf(((float)i / (float)12)))]);
		}


		i++;
	}
}

//Returns true if the given chunk contains an asteroid
int Game::CheckAsteroidChunk(int chunkToCheck)
{
	int returnCount = 0;
	if (asteroids.size() > 0)
	{
		//Count up if any asteroids occupy the chunk
		for (int a = 0; a < asteroids.size(); a++)
		{
			if (asteroids[a]->globalPosition.x > chunk[chunkToCheck].x && asteroids[a]->globalPosition.x < chunk[chunkToCheck].x + chunk[chunkToCheck].width
				&& asteroids[a]->globalPosition.y > chunk[chunkToCheck].y && asteroids[a]->globalPosition.y < chunk[chunkToCheck].y + chunk[chunkToCheck].height)
			{
				returnCount++;
			}
		}
	}

	return returnCount;
}

//Counts how many large asteroids currently exist in the world
int Game::CountLargeAsteroids()
{
	int returnCount = 0;
	for (int a = 0; a < asteroids.size(); a++)
	{
		if (asteroids[a]->size == 3)
		{
			returnCount++;
		}
	}
	return returnCount;
}


void Game::WaveIncrease()
{
	wave += 1;
	bossSpawned = false;
	//Non-boss wave
	if (wave % bossWave != 0)
	{
		//Change enemies per spawn
		if (enemiesPerSpawn < 4
			&& (wave + 1) % 4 == 0)
		{ enemiesPerSpawn += 1; }

		//Change enemy spawn time
		if (enemySpawnTime > 3
			 && (wave + 2) % 4 == 0) //Increase timer each round for the first 3 rounds
		{ enemySpawnTime -= 1; }

		//Reset spawn count
		enemiesToSpawn = enemiesPerWave;
		//Reset spawn timer
		enemySpawnTimer = enemySpawnTime;
	}

	//Destroy small asteroids
	for (int a = 0; a < asteroids.size(); a++)
	{
		if (asteroids[a]->size != 3//Only destroy small asteroids if they're not in camera view
			&& (asteroids[a]->globalPosition.x < cameraPosition.x - 32 || asteroids[a]->globalPosition.x > cameraPosition.x + cameraSize.x + 32
			|| asteroids[a]->globalPosition.y < cameraPosition.y - 32 || asteroids[a]->globalPosition.y > cameraPosition.y + cameraSize.y + 32))
		{
			asteroids[a]->Destroy();
		}
	}
	//Spawn asteroids
	SpawnWaveAsteroids();
}

//Create a root object
void Game::InstanceObject(GameObject* newObj, int posX, int posY)
{
	newObj->parent = nullptr;
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

	PlaySound(sfx_gameover);


	garbageCollection.clear();
	//Destroy everything
	for (int i = 0; i < scene.size(); i++)
	{
		if (!scene[i]->destroyed)
		scene[i]->Destroy();
	}

	//Set references to null
	player = nullptr;
	base = nullptr;

	//Stop timers
	playerSpawnTimer = 0;
	enemySpawnTimer = 0;

	//Create restart button
	InstanceObject(new Button("RESTART", 160, 48), cameraSize.x / 2, (cameraSize.y / 2) + 80);
	//Create quit button
	InstanceObject(new Button("QUIT", 160, 48), cameraSize.x / 2, (cameraSize.y / 2) + 144);
}

void Game::StartGame()
{
	//Reset game variables
	menuOpen = false;
	gameover = false;
	score = 0;
	killCount = 0;
	//Reset wave
	wave = 1;
	enemiesPerWave = 2; //The number of enemies that spawn for each wave before the next wave triggers
	enemiesPerSpawn = 1; //The number of enemies that spawn at once
	enemiesToSpawn = enemiesPerWave; //The number of enemies still left to spawn before a wave increase
	bossSpawned = false;
	powerupSpawn = 1; //The number of medium sized asteroids you have to destroy before a powerup spawns

	//Destroy all objects
	while (scene.size() > 0)
	{
		for (int i = 0; i < scene.size(); i++)
		{
			delete scene[i];
		}
	}

	//Instance objects
	InstanceObject(new Base(), center.x, center.y);
	InstanceObject(new UI(), 0, 0);
	InstanceObject(new Player(), playerSpawn.x, playerSpawn.y);

	//Start enemy spawn timer
	enemySpawnTime = 8;
	enemySpawnTimer = -1; //Pause spawn timer until instructions are finished displaying

	//Create asteroids
	for (int i = 0; i < sizeof(chunk) / sizeof(Rectangle); i++)
	{
		if (i != 5 && i != 6 && i != 9 && i != 10)
		{
			SpawnAsteroid(i);
		}
	}

	//Set start timer
	instructionTimer = instructionTime;

}

void Game::StartMenu()
{
	menuOpen = true;
	//Create start button
	InstanceObject(new Button("START", 160, 48), cameraSize.x / 2, (cameraSize.y / 2) + 16);
	//Create quit button
	InstanceObject(new Button("QUIT", 160, 48), cameraSize.x / 2, (cameraSize.y / 2) + 96);
	//Create fullscreen button
	InstanceObject(new Button("FULLSCREEN", 0.2, 96, 48), cameraSize.x - 96, cameraSize.y - 64);
}


//Returns true if the given vector2 position is within camera range
bool Game::InCamera(Vector2 pos)
{
	return
		(pos.x > cameraPosition.x && pos.x < cameraPosition.x + cameraSize.x &&
		pos.y > cameraPosition.y && pos.y < cameraPosition.y + cameraSize.y);
}



//Deconstructor
Game::~Game()
{
	scene.clear();
	instance = NULL;
}
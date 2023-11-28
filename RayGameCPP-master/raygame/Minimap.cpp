#include "Minimap.h"
#include "Game.h"
#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <string>
using namespace std;

//Constructor
Minimap::Minimap()
{
	GameObject::name = "CollisionShape";
}


void Minimap::Start()
{
	GameObject::Start();

	Game::GetInstance()->minimap = this;

	//Set offset
	offset.x = 8; offset.y = 8;

	//Set map size
	mapSize.x = Game::GetInstance()->worldTileSize.x;
	mapSize.y = Game::GetInstance()->worldTileSize.y;
	mapScale = 1;
}

void Minimap::Draw()
{
	GameObject::Draw();

	//Draw background
	Color backColor; backColor.r = 255; backColor.g = 255; backColor.b = 255; backColor.a = 60;
	DrawRectangle(offset.x, offset.y, mapSize.x * mapScale, mapSize.y * mapScale, backColor);
	//Draw player
	Color playerColor; playerColor.r = 0; playerColor.g = 255; playerColor.b = 0; playerColor.a = 180;
	if (Game::GetInstance()->player != nullptr)
	{ DrawRectangle((Game::GetInstance()->player->globalPosition.x / 32) + offset.x, (Game::GetInstance()->player->globalPosition.y / 32) + offset.y, mapScale * 2, mapScale * 2, playerColor); }
	//Draw enemies
	Color enemyColor; enemyColor.r = 255; enemyColor.g = 0; enemyColor.b = 0; enemyColor.a = 180;
	for (int i = 0; i < Game::GetInstance()->enemies.size(); i++)
	{
		DrawRectangle((Game::GetInstance()->enemies[i]->globalPosition.x / 32) + offset.x, (Game::GetInstance()->enemies[i]->globalPosition.y / 32) + offset.y, mapScale * 2, mapScale * 2, enemyColor);
	}
}

void Minimap::Update()
{
	GameObject::Update();
}
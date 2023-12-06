#include "UI.h"
#include "Game.h"
#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <string>
using namespace std;

//Constructor
UI::UI()
{
	GameObject::name = "CollisionShape";
}


void UI::Start()
{
	GameObject::Start();

	//Set draw order
	drawOrder = 5;

	//Set offset
	offset.x = 8; offset.y = 8;

	//Set map size
	mapSize.x = Game::GetInstance()->worldTileSize.x;
	mapSize.y = Game::GetInstance()->worldTileSize.y;
	mapScale = 1;
}

void UI::Draw()
{

	//DRAW MINIMAP
	//Draw background
	Color backColor; backColor.r = 255; backColor.g = 255; backColor.b = 255; backColor.a = 60;
	DrawRectangle(offset.x, offset.y, mapSize.x * mapScale, mapSize.y * mapScale, backColor);
	//Draw base
	if (Game::GetInstance()->base != nullptr)
	{
		DrawRectangle(mapSize.x / 2 + offset.x - mapScale, mapSize.y / 2 + offset.y - mapScale, mapScale * 2, mapScale * 2, YELLOW);
	}
	//Draw enemies
	Color enemyColor; enemyColor.r = 255; enemyColor.g = 0; enemyColor.b = 0; enemyColor.a = 180;
	for (int i = 0; i < Game::GetInstance()->enemies.size(); i++)
	{
		float sizeMult = (Game::GetInstance()->enemies[i]->name == "EnemyDefault") ? 2 : 3; //Scale of the indicator on the minimap
		DrawRectangle((Game::GetInstance()->enemies[i]->globalPosition.x / 32) + offset.x - mapScale, (Game::GetInstance()->enemies[i]->globalPosition.y / 32) + offset.y - mapScale, mapScale * sizeMult, mapScale * sizeMult, enemyColor);
	}
	//Draw player
	Color playerColor; playerColor.r = 0; playerColor.g = 255; playerColor.b = 0; playerColor.a = 180;
	if (Game::GetInstance()->player != nullptr)
	{ DrawRectangle((Game::GetInstance()->player->globalPosition.x / 32) + offset.x - mapScale, (Game::GetInstance()->player->globalPosition.y / 32) + offset.y - mapScale, mapScale * 2, mapScale * 2, playerColor); }
	
	//Boss text
	if (Game::GetInstance()->bossTextTimer > 0)
	{
		char* text = (char*)"!!! BOSS APPROACHING !!!";
		int textWidth = MeasureText(text, 64);
		DrawText(text, (Game::GetInstance()->cameraSize.x / 2) - (textWidth / 2), Game::GetInstance()->cameraSize.y / 2 - 128, 64, RED);
	}

	//DEBUG Draw asteroids on minimap
	//for (int i = 0; i < Game::GetInstance()->asteroids.size(); i++)
	//{
	//	DrawRectangle((Game::GetInstance()->asteroids[i]->globalPosition.x / 32) + offset.x - mapScale, (Game::GetInstance()->asteroids[i]->globalPosition.y / 32) + offset.y - mapScale, mapScale * 2, mapScale * 2, SKYBLUE);
	//}


	int statXPos = 0;
	//DRAW SCORE
	statXPos = mapSize.x + 16;
	DrawText("SCORE", statXPos, 8, 24, WHITE);
	string scoreString = "5387239572";
	DrawText(TextFormat("%06i", Game::GetInstance()->score), statXPos, 32, 24, WHITE);

	//DRAW BASE HP
	int healthbarWidth = 76;
	int healthbarYPos = 106;
	DrawText("BASE", statXPos, 80, 24, WHITE);
	//Background
	DrawRectangle(statXPos, healthbarYPos - 2, healthbarWidth + 4, 12, GRAY);
	//Filler
	if (Game::GetInstance()->base != nullptr)
	{
		Color fillCol = (Game::GetInstance()->base->hitNotifyTimer == 0) ? GREEN : RED;
		DrawRectangle(statXPos + 2, healthbarYPos, healthbarWidth * ((float)Game::GetInstance()->base->hp / (float)Game::GetInstance()->base->maxHp), 8, fillCol);
		//Hit '!' symbol
		if (Game::GetInstance()->base->hitNotifyTimer > 0)
		{
			Vector2 textPos; textPos.x = statXPos + healthbarWidth + 8; textPos.y = healthbarYPos - 12;
			DrawTextEx(Game::GetInstance()->fnt_gameover, "!", textPos, 32, 0, RED);
		}
	}

	//DRAW INSTRUCTIONS
	int textWidth;
	char* text;
	//Move instructions
	if (Game::GetInstance()->instructionTimer > Game::GetInstance()->instructionTime - Game::GetInstance()->instructionTime / 3)
	{
		text = (char*)"Hold RIGHT mouse button to move";
		textWidth = MeasureText(text, 32);
		DrawText(text, (Game::GetInstance()->cameraSize.x / 2) - (textWidth / 2), Game::GetInstance()->cameraSize.y / 2 - 160, 32, RED);
	} //Shoot instructions
	else if (Game::GetInstance()->instructionTimer > Game::GetInstance()->instructionTime / 3)
	{
		text = (char*)"Hold LEFT mouse button to shoot";
		textWidth = MeasureText(text, 32);
		DrawText(text, (Game::GetInstance()->cameraSize.x / 2) - (textWidth / 2), Game::GetInstance()->cameraSize.y / 2 - 160, 32, RED);
	} //Goal
	else if (Game::GetInstance()->instructionTimer != 0)
	{
		text = (char*)"Protect the BASE in the center of the map";
		textWidth = MeasureText(text, 32);
		DrawText(text, (Game::GetInstance()->cameraSize.x / 2) - (textWidth / 2), Game::GetInstance()->cameraSize.y / 2 - 160, 32, RED);
	}

	GameObject::Draw();
}

void UI::Update()
{
	GameObject::Update();
}
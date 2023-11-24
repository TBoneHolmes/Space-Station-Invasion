#include "EnemyDefault.h"
#include "Game.h"
#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <string>
using namespace std;

//Constructor
EnemyDefault::EnemyDefault()
{
	EnemyDefault::name = "EnemyDefault";
}


void EnemyDefault::Start()
{
	GameObject::Start();

	//Set sprite
	sprite = &Game::GetInstance()->spr_player;
	spriteSize = &Game::GetInstance()->rect_player;
	destination = Game::GetInstance()->rect_player;
	spriteOffset = Vector2(); spriteOffset.x = sprite->height / 2; spriteOffset.y = sprite->width / 2;

	//Create collision shape
	//CollisionShape* cs = new CollisionShape(12, 2, 4);
	InstanceObject(new CollisionShape(12, 2, 4), -4.0f, 0);
}

void EnemyDefault::Draw()
{
	GameObject::Draw();

	//Draw at position
	destination.x = globalPosition.x - Game::GetInstance()->cameraPosition.x; destination.y = globalPosition.y - Game::GetInstance()->cameraPosition.y;
	//Draw player
	DrawTexturePro(*sprite, *spriteSize, destination, spriteOffset, globalRotation, WHITE);
}

void EnemyDefault::Update()
{
	GameObject::Update();
}
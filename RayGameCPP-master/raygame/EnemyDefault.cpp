#include "EnemyDefault.h"
#include "Game.h"
#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <string>
using namespace std;

//Constructor
EnemyDefault::EnemyDefault(Vector2 position)
{
	EnemyDefault::name = "EnemyDefault";
	//Set position
	localPosition = position;
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
	Vector2 csOffset; csOffset.x = -4.0f; csOffset.y = 0;
	CollisionShape* cs = new CollisionShape(12, csOffset, 2, 4);
	InstanceObject(cs);
}

void EnemyDefault::Draw()
{
	GameObject::Draw();

	//Draw at position
	destination.x = globalPosition.x; destination.y = globalPosition.y;
	//Draw player
	DrawTexturePro(*sprite, *spriteSize, destination, spriteOffset, globalRotation, WHITE);
}

void EnemyDefault::Update()
{
	GameObject::Update();
}
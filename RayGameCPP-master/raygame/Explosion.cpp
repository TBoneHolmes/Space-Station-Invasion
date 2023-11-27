#include "Explosion.h"
#include "Game.h"
#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <string>
using namespace std;

//Constructor
Explosion::Explosion()
{
	Explosion::name = "Explosion";
}


void Explosion::Start()
{
	GameObject::Start();

	//Set sprite
	spriteFrames = Game::GetInstance()->frames_explosion;
	sprite = &Game::GetInstance()->spr_explosion;
	spriteSize = &Game::GetInstance()->rect_explosion;
	destination = Game::GetInstance()->rect_explosion;
	spriteOffset = Vector2(); spriteOffset.x = sprite->height / 2; spriteOffset.y = (sprite->width / spriteFrames) / 2;
}

void Explosion::Draw()
{
	GameObject::Draw();

	//Draw at position
	destination.x = globalPosition.x - Game::GetInstance()->cameraPosition.x; destination.y = globalPosition.y - Game::GetInstance()->cameraPosition.y;
	//Draw player
	DrawTexturePro(*sprite, *spriteSize, destination, spriteOffset, globalRotation, WHITE);
}

void Explosion::Update()
{
	GameObject::Update();
}
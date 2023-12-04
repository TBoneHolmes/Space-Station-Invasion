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
	GameObject::name = "Explosion";
}


void Explosion::Start()
{
	GameObject::Start();

	//Set sprite
	spriteFrames = Game::GetInstance()->frames_explosion;
	sprite = &Game::GetInstance()->spr_explosion;
	spriteSize = Game::GetInstance()->rect_explosion;
	destination = Game::GetInstance()->rect_explosion;
	spriteOffset = Vector2(); spriteOffset.x = sprite->height / 2; spriteOffset.y = (sprite->width / spriteFrames) / 2;
	//Animation
	frame = 0;
	animationSpeed = 20;
	animationTimer = 1;
	
	drawOrder = 4;
}

void Explosion::Draw()
{
	//Draw at position
	destination.x = globalPosition.x - Game::GetInstance()->cameraPosition.x; destination.y = globalPosition.y - Game::GetInstance()->cameraPosition.y;
	//Set sprite index
	spriteSize.x = (sprite->width / spriteFrames) * frame;
	//Draw player
	DrawTexturePro(*sprite, spriteSize, destination, spriteOffset, globalRotation, WHITE);

	GameObject::Draw();
}

void Explosion::Update()
{
	GameObject::Update();

	ManageAnimation();
}

void Explosion::ManageAnimation()
{
	//Tick down timer
	animationTimer -= GetFrameTime() * animationSpeed;
	//Timeout
	if (animationTimer <= 0)
	{
		animationTimer = 1;
		frame += 1;
		//Destroy self if out of frame range
		if (frame >= spriteFrames)
		{
			delete this;
		}
	}
}
#include "Powerup.h"
#include "Game.h"
#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <string>
using namespace std;

const float vanishTime = 10.0f;

//Constructor
Powerup::Powerup()
{
	GameObject::name = "Powerup";
}


void Powerup::Start()
{
	GameObject::Start();

	//Set draw order
	drawOrder = 4;

	//Set sprite
	sprite = &Game::GetInstance()->spr_powerup;
	spriteSize = &Game::GetInstance()->rect_powerup;
	destination = Game::GetInstance()->rect_powerup;
	spriteOffset = Vector2(); spriteOffset.x = sprite->height / 2; spriteOffset.y = sprite->width / 2;

	//Create collision shape
	InstanceObject(new CollisionShape(12, 0, 1), 0, 0);
	//Cache collision shape
	cs = (CollisionShape*)children.back();

	//Vanish
	vanishTimer = vanishTime;
	alpha = 255;
}

void Powerup::Draw()
{
	//Set draw color
	Color drawCol = WHITE;
	drawCol.a = alpha;
	//Draw at position
	destination.x = globalPosition.x - Game::GetInstance()->cameraPosition.x; destination.y = globalPosition.y - Game::GetInstance()->cameraPosition.y;
	//Draw bullet
	DrawTexturePro(*sprite, *spriteSize, destination, spriteOffset, globalRotation, drawCol);

	GameObject::Draw();
}

void Powerup::Update()
{
	GameObject::Update();
	ManageTimers();
	CollisionCheck();
}

void Powerup::ManageTimers()
{
	//Vanish
	if (vanishTimer > 0)
	{
		vanishTimer -= GetFrameTime();

		//Timeout
		if (vanishTimer <= 0)
		{
			vanishTimer = 0;
		}
	}


	//Alpha
	if (vanishTimer <= 0)
	{
		alpha -= 300 * GetFrameTime();

		if (alpha <= 0)
		{
			//Destroy self
			delete this;
		}
	}
}

void Powerup::CollisionCheck()
{

	//Check for player
	if (!destroyed && cs->GetOverlappingColliders().size() > 0)
	{
		Collect();
	}
}

void Powerup::Collect()
{
	//Give player a powerup
	Game::GetInstance()->player->powerupTimer = Game::GetInstance()->player->powerupTime;
	//Play sfx
	PlaySound(Game::GetInstance()->sfx_powerup);
	//Destroy self
	Destroy();
}
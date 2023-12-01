#include "Powerup.h"
#include "Game.h"
#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <string>
using namespace std;

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
}

void Powerup::Draw()
{
	//Draw at position
	destination.x = globalPosition.x - Game::GetInstance()->cameraPosition.x; destination.y = globalPosition.y - Game::GetInstance()->cameraPosition.y;
	//Draw bullet
	DrawTexturePro(*sprite, *spriteSize, destination, spriteOffset, globalRotation, WHITE);

	GameObject::Draw();
}

void Powerup::Update()
{
	GameObject::Update();
	CollisionCheck();
}

void Powerup::CollisionCheck()
{

	//Check for player
	if (cs->GetOverlappingColliders().size() > 0)
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
	GameObject* ptr = this;
	ptr->~GameObject();
}
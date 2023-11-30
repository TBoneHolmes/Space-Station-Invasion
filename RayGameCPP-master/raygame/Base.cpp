#include "Base.h"
#include "Game.h"
#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <string>
using namespace std;

//Constructor
Base::Base()
{
	GameObject::name = "Base";
}


void Base::Start()
{
	GameObject::Start();

	//Cache self in game manager
	Game::GetInstance()->base = this;

	//Set draw order
	drawOrder = 0;

	//Set sprite
	sprite = &Game::GetInstance()->spr_base;
	spriteSize = &Game::GetInstance()->rect_base;
	destination = Game::GetInstance()->rect_base;
	spriteOffset = Vector2(); spriteOffset.x = sprite->height / 2; spriteOffset.y = sprite->width / 2;

	//Create collision shape
	InstanceObject(new CollisionShape(80, 16, 2 + 8), 0, 0);
	//Cache collision shape
	cs = (CollisionShape*)children.back();

	//Set HP
	maxHp = 30;
	hp = maxHp;
	damageRest = 0.1;
}

void Base::Draw()
{
	//Set draw position
	destination.x = globalPosition.x - Game::GetInstance()->cameraPosition.x; destination.y = globalPosition.y - Game::GetInstance()->cameraPosition.y;
	//Set draw color
	Color drawCol = WHITE;
	if (damageRestTimer > 0)
	{
		drawCol = RED;
	}
	//Draw base
	DrawTexturePro(*sprite, *spriteSize, destination, spriteOffset, globalRotation, drawCol);

	GameObject::Draw();
}

void Base::Update()
{
	GameObject::Update();

	ManageTimers();
	CollisionCheck();
	//Apply torque
	localRotation += 0.1;
}

void Base::ManageTimers()
{
	//Tick down damage rest timer
	if (damageRestTimer > 0)
	{
		damageRestTimer -= GetFrameTime();
	} //Clamp damageRestTimer to 0
	else { damageRestTimer = 0; }
}

void Base::CollisionCheck()
{
	//Check for bullet
	if (damageRestTimer == 0 && cs->GetOverlappingColliders().size() > 0)
	{
		//TODO: Change this to interact with each collider, not just the first index
		//Hit by bullet
		if (cs->GetOverlappingColliders()[0]->parent->name == "Bullet")
		{
			cs->GetOverlappingColliders()[0]->parent->~GameObject();
			Damage(1);
		}
		else if (cs->GetOverlappingColliders()[0]->parent->name == "EnemyDefault") //Hit by enemy body
		{
			//Create explosion for the enemy that hit
			Game::GetInstance()->InstanceObject(new Explosion(), cs->GetOverlappingColliders()[0]->parent->globalPosition.x, cs->GetOverlappingColliders()[0]->parent->globalPosition.y);
			cs->GetOverlappingColliders()[0]->parent->~GameObject();
			Damage(4);
		}
	}
}

void Base::Damage(int dmg)
{
	hp -= dmg;
	//Set invulnerability timer
	damageRestTimer = damageRest;
	//Check for death
	if (hp <= 0)
	{ Die(); }
	else
	{ PlaySound(Game::GetInstance()->sfx_hitBase); }
}

void Base::Die()
{
	//Create explosion
	Game::GetInstance()->InstanceObject(new Explosion, globalPosition.x + 2.0f, globalPosition.y);
	//Gameover
	Game::GetInstance()->Gameover();
	//Destroy self
	GameObject* ptr = this;
	ptr->~GameObject();
}
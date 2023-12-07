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

	//Damage
	maxHp = 43;
	hp = maxHp;
	damageRest = 0.1;
	hitNotifyTime = 1;
	hitNotifyTimer = 0;
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
	} //Timeout
	else { damageRestTimer = 0; }

	//Hit notifier
	if (hitNotifyTimer > 0)
	{
		hitNotifyTimer -= GetFrameTime();
	}//Timeout
	else { hitNotifyTimer = 0; }
}

void Base::CollisionCheck()
{
	//Check for bullet
	if (damageRestTimer == 0 && cs->GetOverlappingColliders().size() > 0)
	{
		for (int i = 0; i < cs->GetOverlappingColliders().size(); i++)
		{
			//Hit by bullet
			if (cs->GetOverlappingColliders()[i]->parent->name == "Bullet")
			{
				Damage(1);
				if (hp > 0) //Destroy bullet that hit
				{ cs->GetOverlappingColliders()[i]->parent->Destroy(); }
			}
			//Hit by boss
			else if (cs->GetOverlappingColliders()[i]->parent->name == "Boss")
			{
				Die();
			}
			//Hit by enemy body
			else if (cs->GetOverlappingColliders()[i]->parent->name == "EnemyDefault")
			{
				Damage(4);
				//Destroy enemy that hit
				if (hp > 0)
				{
					Game::GetInstance()->InstanceObject(new Explosion(), cs->GetOverlappingColliders()[0]->parent->globalPosition.x, cs->GetOverlappingColliders()[0]->parent->globalPosition.y);
					cs->GetOverlappingColliders()[i]->parent->Destroy();
				}
			}
		}
	}
}

void Base::Damage(int dmg)
{
	hp -= dmg;
	//Set invulnerability timer
	damageRestTimer = damageRest;
	hitNotifyTimer = hitNotifyTime;
	//Check for death
	if (hp <= 0)
	{ Die(); }
	else
	{ PlaySound(Game::GetInstance()->sfx_hitBase); }
}

void Base::Die()
{
	if (!Game::GetInstance()->gameover)
	{
		//Gameover
		Game::GetInstance()->Gameover();
	}
}
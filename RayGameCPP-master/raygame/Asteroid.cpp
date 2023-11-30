#include "Asteroid.h"
#include "Game.h"
#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <string>
using namespace std;

//Constructor
Asteroid::Asteroid()
{
	GameObject::name = "Asteroid";
	size = 3;
	creator = nullptr;
}
Asteroid::Asteroid(int s, Asteroid* c)
{
	GameObject::name = "Asteroid";
	size = s;
	creator = c;
}

void Asteroid::Start()
{
	GameObject::Start();

	//Add self to the game manager's CollisionShapes list
	Game::GetInstance()->asteroids.push_back(this);

	//Set sprite
	if (size == 1) //Small asteroid
	{
		sprite = &Game::GetInstance()->spr_asteroidS;
		spriteSize = &Game::GetInstance()->rect_asteroidS;
		destination = Game::GetInstance()->rect_asteroidS;
	}else if (size == 2) //Med asteroid
	{
		sprite = &Game::GetInstance()->spr_asteroidM;
		spriteSize = &Game::GetInstance()->rect_asteroidM;
		destination = Game::GetInstance()->rect_asteroidM;
	}else if (size == 3) //Big-ass asteriod
	{
		sprite = &Game::GetInstance()->spr_asteroidL;
		spriteSize = &Game::GetInstance()->rect_asteroidL;
		destination = Game::GetInstance()->rect_asteroidL;
	}
	spriteOffset = Vector2(); spriteOffset.x = sprite->height / 2; spriteOffset.y = sprite->width / 2;

	//Create collision shape
	int csSize = 16 * size;
	if (size == 3)
	{ csSize += 16; }
	InstanceObject(new CollisionShape(csSize, 2, 2 + 4), 0, 0);
	//Cache collision shape
	cs = (CollisionShape*)children.back();

	//Set score
	killScore = 1;

	//Set damage timer
	damageRest = 0.1;
	damageRestTimer = 0;

	//Set bounce timer
	bounceTimer = 0;

	hp = size;

	//Set speed and direction
	speed = GetRandomValue(2, 10) * 0.1;
	int direction = GetRandomValue(0, 359);
	velocity.x = speed; velocity.y = 0;
	velocity = Vector2Rotate(velocity, direction);

	//Set rotation and torque
	localRotation = GetRandomValue(0, 359);
	torque = (GetRandomValue(-5, 5) * 0.1);
}

void Asteroid::Draw()
{
	//Draw at position
	destination.x = globalPosition.x - Game::GetInstance()->cameraPosition.x; destination.y = globalPosition.y - Game::GetInstance()->cameraPosition.y;
	//Set draw color
	Color drawCol = WHITE;
	if (damageRestTimer > 0)
	{ drawCol = RED; }
	//Draw asteroid
	DrawTexturePro(*sprite, *spriteSize, destination, spriteOffset, globalRotation, drawCol);

	GameObject::Draw();
}

void Asteroid::Update()
{
	GameObject::Update();

	ManageTimers();
	ApplyVelocity();
	CollisionCheck();
}

void Asteroid::ManageTimers()
{
	//Damage rest timer
	if (damageRestTimer > 0)
	{
		damageRestTimer -= GetFrameTime();
		//Timeout
		if (damageRestTimer <= 0)
		{
			damageRestTimer = 0;
		}
	}

	//Bounce timer
	if (bounceTimer > 0)
	{
		bounceTimer -= GetFrameTime();
		//Timeout
		if (bounceTimer <= 0)
		{
			//Reset timer if still colliding with an asteroid
			for (int i = 0; i < cs->GetOverlappingColliders().size(); i++)
			{
				if (cs->GetOverlappingColliders()[i]->parent->name == "Asteroid")
				{
					Asteroid* asteroidCollider = (Asteroid*)cs->GetOverlappingColliders()[i]->parent;
					if (creator == nullptr || asteroidCollider->creator != creator)
					{
						bounceTimer = 1;
					}
					else
					{
						bounceTimer = 0;
					}
					
				}
			}
			//Not colliding with an asteroid; stop timer
			bounceTimer = 0;
		}
	}
}

void Asteroid::ApplyVelocity()
{
	//Apply velocity
	localPosition = Vector2Add(localPosition, velocity);

	//Apply torque
	localRotation += torque;
}


//DAMAGE

void Asteroid::CollisionCheck()
{
	//Check collision
	if (cs->GetOverlappingColliders().size() > 0)
	{
		for (int i = 0; i < cs->GetOverlappingColliders().size(); i++)
		{
			//Collision with bullet
			if (cs->GetOverlappingColliders()[i]->parent->name == "Bullet" && damageRestTimer == 0)
			{
				cout << "Destroyed bullet" << endl;
				//Destroy bullet
				cs->GetOverlappingColliders()[i]->parent->~GameObject();
				//Damage self
				Damage(1);
			}
			//Collision with asteroid
			else if (cs->GetOverlappingColliders()[i]->parent->name == "Asteroid" && bounceTimer == 0)
			{
				//Only bounce if asteroid wasn't created by the same larger asteroid
				Asteroid* asteroidCollider = (Asteroid*)cs->GetOverlappingColliders()[i]->parent;
				if (creator == nullptr || asteroidCollider->creator != creator)
				{
					velocity = Vector2Scale(velocity, -1);
					bounceTimer = 0.2;
				}
			}

		}
	}

	//Bounce off edge of screen
	if (globalPosition.x + velocity.x < sprite->width / 2 || globalPosition.x + velocity.x > Game::GetInstance()->worldSize.x - (sprite->width / 2)
		|| globalPosition.y + velocity.y < sprite->height / 2 || globalPosition.y + velocity.y > Game::GetInstance()->worldSize.y - (sprite->height / 2)
		&& bounceTimer == 0)
	{
		velocity = Vector2Scale(velocity, -1);
		bounceTimer = 0.1;
	}
}

void Asteroid::Damage(int dmg)
{
	hp -= dmg;
	//Set invulnerability timer
	damageRestTimer = damageRest;
	//Die when hp reaches 0
	if (hp <= 0)
	{
		Game::GetInstance()->score += killScore * size;
		Die();
	}
	else 
	{ PlaySound(Game::GetInstance()->sfx_hitAsteroid); }
}

void Asteroid::Die()
{
	//Create new asteroids
	if (size > 1)
	{
		Vector2 spawnPos;
		spawnPos = Vector2Normalize(Vector2Rotate(velocity, 90));
		for (int i = 0; i < 2; i++)
		{ 
			Asteroid* newCreator = (creator == nullptr) ? this : creator;
			Game::GetInstance()->InstanceObject(new Asteroid(size - 1, newCreator), globalPosition.x + (spawnPos.x * (sprite->width / 4)), globalPosition.y + (spawnPos.y * (sprite->width / 4)));
			spawnPos = Vector2Scale(spawnPos, -1);
		}
	}
	PlaySound(Game::GetInstance()->sfx_explodeAsteroid);
	//Create explosion
	Game::GetInstance()->InstanceObject(new Explosion(), globalPosition.x + 2.0f, globalPosition.y);
	//Destroy self
	GameObject* ptr = this;
	ptr->~GameObject();
}
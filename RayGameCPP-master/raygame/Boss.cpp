#include "Boss.h"
#include "BossShield.h"
#include "EnemyDefault.h"
#include "ScoreNotifier.h"
#include "Game.h"
#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <string>
using namespace std;

//Constructor
Boss::Boss()
{
	GameObject::name = "Boss";
}


void Boss::Start()
{
	GameObject::Start();

	//Add self to the game manager's CollisionShapes list
	Game::GetInstance()->enemies.push_back(this);

	//Set draw order
	drawOrder = 2;

	//Set sprite
	sprite = &Game::GetInstance()->spr_boss;
	spriteSize = &Game::GetInstance()->rect_boss;
	destination = Game::GetInstance()->rect_boss;
	spriteOffset = Vector2(); spriteOffset.x = sprite->height / 2; spriteOffset.y = sprite->width / 2;

	//Create collision shape
	InstanceObject(new CollisionShape(24, 2, 4), 0, 0);
	//Cache collision shape
	cs = (CollisionShape*)children.back();

	//Create shield bots
	InstanceObject(new BossShield(0), 0, 0);
	InstanceObject(new BossShield(30), 0, 0);
	InstanceObject(new BossShield(60), 0, 0);
	InstanceObject(new BossShield(90), 0, 0);
	InstanceObject(new BossShield(120), 0, 0);
	InstanceObject(new BossShield(150), 0, 0);
	InstanceObject(new BossShield(180), 0, 0);
	InstanceObject(new BossShield(210), 0, 0);
	InstanceObject(new BossShield(240), 0, 0);
	InstanceObject(new BossShield(270), 0, 0);
	//InstanceObject(new BossShield(300), 0, 0);
	//InstanceObject(new BossShield(330), 0, 0);

	//Create enemy homies
	for (int i = 0; i < Game::GetInstance()->wave / Game::GetInstance()->bossWave; i++)
	{
		float spawnAngle = GetRandomValue(0, 360);
		Vector2 spawnDist = Vector2Zero(); spawnDist.x = 96;
		Vector2 spawnPos = Vector2Add(localPosition, Vector2Rotate(spawnDist, spawnAngle));
		Game::GetInstance()->InstanceObject(new EnemyDefault(), spawnPos.x, spawnPos.y);
	}

	//Set HP
	hp = 10;
	damageRest = 0.1;

	//Set movement values
	maxSpeed = (60) + Game::GetInstance()->wave;
	acceleration = 4;

	//Set shoot values
	shootRest = 1;
	shootRestTimer = 0;

	//Set score
	killScore = 500;
}

void Boss::Draw()
{

	//Draw at position
	destination.x = globalPosition.x - Game::GetInstance()->cameraPosition.x; destination.y = globalPosition.y - Game::GetInstance()->cameraPosition.y;
	//Set draw color
	Color drawCol = (damageRestTimer > 0) ? RED : WHITE;
	//Draw boss
	DrawTexturePro(*sprite, *spriteSize, destination, spriteOffset, globalRotation, drawCol);

	GameObject::Draw();
}

void Boss::Update()
{
	GameObject::Update();
	if (hp > 0)
	{

		ManageTimers();
		CollisionCheck();
		MoveToPoint();
		ApplyVelocity();
	}

	//Die if hp is 0 and no shield objects exist
	if (hp <= 0 && children.size() <= 1)
	{
		Game::GetInstance()->score += killScore;
		Die();
	}
}

void Boss::ManageTimers()
{
	//Tick down timer
	if (shootRestTimer > 0)
	{
		shootRestTimer -= GetFrameTime();
	}
	//Clamp timer to 0
	else { shootRestTimer = 0; }

	//Tick down damage rest timer
	if (damageRestTimer > 0)
	{
		damageRestTimer -= GetFrameTime();
	} //Clamp damageRestTimer to 0
	else { damageRestTimer = 0; }
}

//DAMAGE

void Boss::CollisionCheck()
{
	//Check for bullet
	if (damageRestTimer == 0 && cs->GetOverlappingColliders().size() > 0)
	{
		//Destroy bullet
		cs->GetOverlappingColliders()[0]->parent->~GameObject();
		//Damage self
		Damage(1);
	}
}

void Boss::Damage(int dmg)
{
	hp -= dmg;
	//Set invulnerability timer
	damageRestTimer = damageRest;

	if (hp <= 0)
	{

	}
	else
	{
		Game::GetInstance()->screenshake = 5;
		PlaySound(Game::GetInstance()->sfx_hitEnemy);
	}
}

void Boss::Die()
{
	Game::GetInstance()->killCount += 1;
	Game::GetInstance()->screenshake = 10;
	Game::GetInstance()->freeze = 0.10;
	PlaySound(Game::GetInstance()->sfx_explodeEnemy);
	PlaySound(Game::GetInstance()->sfx_explodeBoss);
	//Create explosion
	Game::GetInstance()->InstanceObject(new Explosion, globalPosition.x, globalPosition.y);
	//Create scoreNotifier
	Game::GetInstance()->InstanceObject(new ScoreNotifier(killScore), globalPosition.x, globalPosition.y);
	//Go to next wave
	Game::GetInstance()->WaveIncrease();
	//Destroy self
	Destroy();
}

//MOVEMENT
//Enemy moves toward the base/center
void Boss::MoveToPoint()
{
	//Get the angle to point to
	float targetDirection = Vector2Angle(Vector2Subtract(globalPosition, Game::GetInstance()->cameraPosition), Vector2Subtract(Game::GetInstance()->center, Game::GetInstance()->cameraPosition));
	//Set rotation
	localRotation = targetDirection;

	Accelerate();
}

//Apply acceleration to the velocity
void Boss::Accelerate()
{
	if (Vector2Length(Vector2Add(velocity, Vector2Rotate(Vector2Right, globalRotation))) < maxSpeed)
	{
		velocity = Vector2Lerp(velocity, Vector2Scale(Vector2Rotate(Vector2Right, globalRotation), maxSpeed), acceleration * 0.01);
	}
}

//Apply velocity to the object's position
void Boss::ApplyVelocity()
{
	localPosition = Vector2Add(localPosition, Vector2Scale(velocity, GetFrameTime()));
	//Clamp position
	localPosition.x = Clamp(localPosition.x, -32, Game::GetInstance()->worldSize.x + 32);
	localPosition.y = Clamp(localPosition.y, -32, Game::GetInstance()->worldSize.y + 32);
}

//SHOOT
void Boss::Shoot()
{
	//Shoot
	if (shootRestTimer == 0)
	{
		shootRestTimer = shootRest;
		//Spawn bullet
		Vector2 bulletSpawnPos = Vector2Add(globalPosition, Vector2Rotate(Vector2Scale(Vector2Right, sprite->width / 2), globalRotation));
		Game::GetInstance()->InstanceObject(new Bullet(Vector2Rotate(Vector2Right, globalRotation), 8), bulletSpawnPos.x, bulletSpawnPos.y);
		//Play sfx
		PlaySound(Game::GetInstance()->sfx_shootEnemy);
	}
}

void Boss::Destroy()
{
	GameObject::Destroy();

	GameObject* ptr = this;
	auto iter = Game::GetInstance()->enemies.begin();
	for (int i = 0; i < Game::GetInstance()->enemies.size(); i++)
	{
		if (Game::GetInstance()->enemies[i] == ptr)
		{
			Game::GetInstance()->enemies.erase(iter);
			break;
		}
		iter++;
	}
}
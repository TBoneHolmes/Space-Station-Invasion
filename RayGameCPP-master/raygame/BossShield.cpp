#include "BossShield.h"
#include "ScoreNotifier.h"
#include "Game.h"
#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <string>
using namespace std;

//Constructor
BossShield::BossShield()
{
	GameObject::name = "BossShield";
}

void BossShield::Start()
{
	GameObject::Start();

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

	//Set HP
	hp = 10;
	damageRest = 0.1;

	//Set movement values
	maxSpeed = 2;
	acceleration = 4;

	//Set shoot values
	shootRest = 1;
	shootRestTimer = 0;

	//Set score
	killScore = 500;
}

void BossShield::Draw()
{

	//Draw at position
	destination.x = globalPosition.x - Game::GetInstance()->cameraPosition.x; destination.y = globalPosition.y - Game::GetInstance()->cameraPosition.y;
	//Set draw color
	Color drawCol = (damageRestTimer > 0) ? RED : WHITE;
	//Draw boss
	DrawTexturePro(*sprite, *spriteSize, destination, spriteOffset, globalRotation, drawCol);

	GameObject::Draw();
}

void BossShield::Update()
{
	GameObject::Update();

	ManageTimers();
	CollisionCheck();
	MoveToPoint();
	ApplyVelocity();
}

void BossShield::ManageTimers()
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

void BossShield::CollisionCheck()
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

void BossShield::Damage(int dmg)
{
	Game::GetInstance()->screenshake = 5;
	hp -= dmg;
	//Set invulnerability timer
	damageRestTimer = damageRest;
	//Die when hp reaches 0
	if (hp <= 0)
	{
		Game::GetInstance()->score += killScore;
		Die();
	}
	else
	{
		PlaySound(Game::GetInstance()->sfx_hitEnemy);
	}
}

void BossShield::Die()
{
	Game::GetInstance()->freeze = 0.12;
	PlaySound(Game::GetInstance()->sfx_explodeEnemy);
	//Create explosion
	Game::GetInstance()->InstanceObject(new Explosion, globalPosition.x, globalPosition.y);
	//Create scoreNotifier
	Game::GetInstance()->InstanceObject(new ScoreNotifier(killScore), globalPosition.x, globalPosition.y);
	//Destroy self
	delete this;
}

//MOVEMENT
//Enemy moves toward the base/center
void BossShield::MoveToPoint()
{
	//Get the angle to point to
	float targetDirection = Vector2Angle(Vector2Subtract(globalPosition, Game::GetInstance()->cameraPosition), Vector2Subtract(Game::GetInstance()->center, Game::GetInstance()->cameraPosition));
	//Set rotation
	localRotation = targetDirection;

	Accelerate();
}

//Apply acceleration to the velocity
void BossShield::Accelerate()
{
	if (Vector2Length(Vector2Add(velocity, Vector2Rotate(Vector2Right, globalRotation))) < maxSpeed)
	{
		velocity = Vector2Lerp(velocity, Vector2Scale(Vector2Rotate(Vector2Right, globalRotation), maxSpeed), acceleration * 0.01);
	}
}

//Apply velocity to the object's position
void BossShield::ApplyVelocity()
{
	localPosition = Vector2Add(localPosition, velocity);
	//Clamp position
	localPosition.x = Clamp(localPosition.x, -32, Game::GetInstance()->worldSize.x + 32);
	localPosition.y = Clamp(localPosition.y, -32, Game::GetInstance()->worldSize.y + 32);
}

//SHOOT
void BossShield::Shoot()
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
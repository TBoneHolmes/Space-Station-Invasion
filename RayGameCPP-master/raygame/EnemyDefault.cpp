#include "EnemyDefault.h"
#include "ScoreNotifier.h"
#include "Game.h"
#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <string>
using namespace std;

//Constructor
EnemyDefault::EnemyDefault()
{
	GameObject::name = "EnemyDefault";
}


void EnemyDefault::Start()
{
	GameObject::Start();

	//Add self to the game manager's CollisionShapes list
	Game::GetInstance()->enemies.push_back(this);

	//Set draw order
	drawOrder = 2;

	//Set sprite
	sprite = &Game::GetInstance()->spr_enemyDefault;
	spriteSize = &Game::GetInstance()->rect_enemyDefault;
	destination = Game::GetInstance()->rect_enemyDefault;
	spriteOffset = Vector2(); spriteOffset.x = sprite->height / 2; spriteOffset.y = sprite->width / 2;

	//Create collision shape
	InstanceObject(new CollisionShape(12, 2, 4), 2.0f, 0);
	//Cache collision shape
	cs = (CollisionShape*)children.back();

	//Set HP
	hp = 2;
	damageRest = 0.1;

	//Set movement values
	targetPoint = Game::GetInstance()->center;
	maxSpeed = GetRandomValue(3, 6);
	acceleration = GetRandomValue(4, 8);

	//Set shoot values
	shootRest = 1;
	shootRestTimer = 0;
	
	//Set score
	killScore = 50;
}

void EnemyDefault::Draw()
{

	//Draw at position
	destination.x = globalPosition.x - Game::GetInstance()->cameraPosition.x; destination.y = globalPosition.y - Game::GetInstance()->cameraPosition.y;
	//Set draw color
	Color drawCol = WHITE;
	if (damageRestTimer > 0)
	{
		drawCol = RED;
	}
	//Draw player
	DrawTexturePro(*sprite, *spriteSize, destination, spriteOffset, globalRotation, drawCol);

	GameObject::Draw();
}

void EnemyDefault::Update()
{
	GameObject::Update();

	ManageTimers();
	Ai();
	CollisionCheck();
	MoveToPoint();
	ApplyVelocity();
}

void EnemyDefault::ManageTimers()
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

void EnemyDefault::Ai()
{
	Vector2 lastTargetPoint = targetPoint;
	//MOVE AI
	//Set target position as player position
	if (Game::GetInstance()->player != nullptr
		&& Vector2Length(Vector2Subtract(Game::GetInstance()->player->globalPosition, globalPosition)) < 384)
	{
		targetPoint = Game::GetInstance()->player->globalPosition;
	}
	else //Set target position as world center
	{
		targetPoint = Game::GetInstance()->center;
	}
	//On targetPoint change, reset shoot timer
	if ((targetPoint.x != lastTargetPoint.x || targetPoint.y != lastTargetPoint.y)
		&& (lastTargetPoint.x == Game::GetInstance()->center.x && lastTargetPoint.y == Game::GetInstance()->center.y))
	{ shootRestTimer = shootRest; }

	//SHOOT AI
	if (Game::GetInstance()->player != nullptr &&
		targetPoint.x == Game::GetInstance()->player->globalPosition.x
		&& targetPoint.y == Game::GetInstance()->player->globalPosition.y)
	{
		Shoot();
	}
}

//DAMAGE

void EnemyDefault::CollisionCheck()
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

void EnemyDefault::Damage(int dmg)
{
	Game::GetInstance()->screenshake = 5;
	hp -= dmg;
	//Set invulnerability timer
	damageRestTimer = damageRest;
	//Die when hp reaches 0
	if (hp <= 0)
	{
		Game::GetInstance()->freeze = 0.08;
		Game::GetInstance()->score += killScore;
		Die();
	}
	else
	{ PlaySound(Game::GetInstance()->sfx_hitEnemy); }
}

void EnemyDefault::Die()
{
	Game::GetInstance()->freeze = 0.12;
	PlaySound(Game::GetInstance()->sfx_explodeEnemy);
	//Create explosion
	Game::GetInstance()->InstanceObject(new Explosion, globalPosition.x + 2.0f, globalPosition.y);
	//Create scoreNotifier
	Game::GetInstance()->InstanceObject(new ScoreNotifier(killScore), globalPosition.x, globalPosition.y);
	//Destroy self
	GameObject* ptr = this;
	ptr->~GameObject();
}

//MOVEMENT
//Enemy moves toward the base/center
void EnemyDefault::MoveToPoint()
{
	//Get the angle to point to
	float targetDirection = Vector2Angle(Vector2Subtract(globalPosition, Game::GetInstance()->cameraPosition), Vector2Subtract(targetPoint, Game::GetInstance()->cameraPosition));
	//Set rotation
	localRotation = targetDirection;

	Accelerate();
}

//Apply acceleration to the velocity
void EnemyDefault::Accelerate()
{
	if (Vector2Length(Vector2Add(velocity, Vector2Rotate(Vector2Right, globalRotation))) < maxSpeed)
	{
		velocity = Vector2Lerp(velocity, Vector2Scale(Vector2Rotate(Vector2Right, globalRotation), maxSpeed), acceleration * 0.01);
	}
}

//Apply velocity to the object's position
void EnemyDefault::ApplyVelocity()
{
	localPosition = Vector2Add(localPosition, velocity);
	//Clamp position
	localPosition.x = Clamp(localPosition.x, -32, Game::GetInstance()->worldSize.x + 32);
	localPosition.y = Clamp(localPosition.y, -32, Game::GetInstance()->worldSize.y + 32);
}

//SHOOT
void EnemyDefault::Shoot()
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
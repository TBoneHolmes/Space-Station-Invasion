#include "BossShield.h"
#include "ScoreNotifier.h"
#include "Game.h"
#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <string>
using namespace std;

//Constructor
BossShield::BossShield(float startAngle)
{
	GameObject::name = "BossShield";
	targetAngle = startAngle;
}

void BossShield::Start()
{
	GameObject::Start();

	//Set draw order
	drawOrder = 2;

	//Set sprite
	sprite = &Game::GetInstance()->spr_bossShield;
	spriteSize = &Game::GetInstance()->rect_bossShield;
	destination = Game::GetInstance()->rect_bossShield;
	spriteOffset = Vector2(); spriteOffset.x = sprite->height / 2; spriteOffset.y = sprite->width / 2;

	//Create collision shape
	InstanceObject(new CollisionShape(12, 2, 4), 0, 0);
	//Cache collision shape
	cs = (CollisionShape*)children.back();

	//Set HP
	hp = 3;
	damageRest = 0.1;

	//Set shoot values
	shootRest = 7;
	shootRestTimer = GetRandomValue(0, shootRest);

	//Set score
	killScore = 10;

	//Position/movement values
	targetDist = 64;
	speed = 160;
	rotationSpeed = 400; //For sprite rotation
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

	SetPosition();
	ApplyMovement();
	ManageTimers();
	CollisionCheck();

	//Kill if parent is dead
	Boss* bossParent = (Boss*)parent;
	if (bossParent->hp <= 0)
	{ Die(); }
}

void BossShield::ManageTimers()
{
	//Tick down timer
	if (shootRestTimer > 0
		&& Game::GetInstance()->player != nullptr //Player exists
		&& Game::GetInstance()->InCamera(globalPosition)) //This shield piece is within camera bounds
		//&& Vector2Length(Vector2Subtract(Game::GetInstance()->player->globalPosition, globalPosition)) < 384) //Player is in range
	{
		shootRestTimer -= GetFrameTime();

		//Timeout
		if (shootRestTimer <= 0)
		{
			shootRestTimer = shootRest;
			Shoot();
		}
	}


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
	hp -= dmg;
	//Set invulnerability timer
	damageRestTimer = damageRest;
	//Die when hp reaches 0
	if (hp <= 0)
	{
		Die();
	}
	else
	{
		Game::GetInstance()->screenshake = 3;
		PlaySound(Game::GetInstance()->sfx_hitEnemy);
	}
}

void BossShield::Die()
{
	//Set killscore to 1 and play sfx if an individual shield is destroyed
	Boss* bossParent = (Boss*)parent;
	if (bossParent->hp > 0)
	{
		killScore = 1;
		Game::GetInstance()->screenshake = 5;
		PlaySound(Game::GetInstance()->sfx_explodeEnemy);
	}
	Game::GetInstance()->score += killScore;

	Game::GetInstance()->freeze = 0.12;
	//Create explosion
	Game::GetInstance()->InstanceObject(new Explosion, globalPosition.x, globalPosition.y);
	//Create scoreNotifier
	Game::GetInstance()->InstanceObject(new ScoreNotifier(killScore), globalPosition.x, globalPosition.y);
	//Destroy self
	Destroy();
}

//MOVEMENT
void BossShield::SetPosition()
{
	Vector2 pos; pos.x = targetDist; pos.y = 0;
	localPosition = Vector2Rotate(pos, targetAngle);
}

void BossShield::ApplyMovement()
{
	targetAngle += speed * GetFrameTime();
	//Clamp angle
	if (targetAngle > 360)
	{ targetAngle -= 360; }

	//Apply sprite rotation
	localRotation += rotationSpeed * GetFrameTime();
}

//SHOOT
void BossShield::Shoot()
{
	//Spawn bullet
	int positionRandom = 64; //The amount of random position variation from the player position to shoot towards
	Vector2 targetPosition; targetPosition.x = GetRandomValue(-positionRandom, positionRandom); targetPosition.y = GetRandomValue(-positionRandom, positionRandom);
	float targetDirection = Vector2Angle(globalPosition, Vector2Add(Game::GetInstance()->player->globalPosition, targetPosition));
	Game::GetInstance()->InstanceObject(new Bullet(Vector2Rotate(Vector2Right, targetDirection), 8), globalPosition.x, globalPosition.y);
	//Play sfx
	PlaySound(Game::GetInstance()->sfx_shootEnemy);
}
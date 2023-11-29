#include "Game.h"
#include "Player.h"
#include "Bullet.h"
#include "CollisionShape.h"
#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <string>
using namespace std;

//Constructor
Player::Player()
{
	GameObject::name = "Player";
}


void Player::Start()
{
	GameObject::Start();

	//Set player reference
	Game::GetInstance()->player = this;
	//Set camera
	Game::GetInstance()->cameraOwner = this;

	//Set draw order
	drawOrder = 3;

	//Set sprite
	sprite = &Game::GetInstance()->spr_player;
	spriteSize = &Game::GetInstance()->rect_player;
	destination = Game::GetInstance()->rect_player;
	spriteOffset = Vector2(); spriteOffset.x = sprite->height/2; spriteOffset.y = sprite->width/2;


	//Create collision shape
	InstanceObject(new CollisionShape(12, 1, 2 + 8), -4.0, 0);
	//Cache collision shape
	cs = (CollisionShape*)children.back();


	//Set movement values
	maxSpeed = 8;
	acceleration = 0.4;
	decceleration = 1;

	//Set key binds
	key_boost = MOUSE_RIGHT_BUTTON;
	key_shoot = MOUSE_LEFT_BUTTON;

	//Set HP
	maxHp = 3;
	hp = maxHp;
	damageRest = 0.1;

	//Set shoot timer
	shootRest = 0.3;
	shootRestTimer = 0;

	//Set invincible timer
	invTime = 1;
	invTimer = invTime;

	//DESTROY SELF
	//GameObject* ptr = this;
	//ptr->~GameObject();
}

void Player::Draw()
{
	//Draw at position
	destination.x = globalPosition.x - Game::GetInstance()->cameraPosition.x; destination.y = globalPosition.y - Game::GetInstance()->cameraPosition.y;
	//Set draw color
	Color drawCol = WHITE;
	if (invTimer > 0)
	{
		drawCol.a = 180;
	}
	else if (damageRestTimer > 0)
	{
		drawCol = RED;
	}
	//Draw player
	DrawTexturePro(*sprite, *spriteSize, destination, spriteOffset, globalRotation, drawCol);

	//Draw HP
	Rectangle hpSize; hpSize.x = globalPosition.x - Game::GetInstance()->cameraPosition.x; hpSize.y = globalPosition.y - Game::GetInstance()->cameraPosition.y; hpSize.width = 16 / maxHp; hpSize.height = sprite->height / maxHp;
	Vector2 hpOffset; hpOffset.x = sprite->width / 2 + 6; hpOffset.y = hpSize.height + 2;
	if (hp > 1)
	{
		DrawRectanglePro(hpSize, hpOffset, globalRotation, GREEN);
	} if (hp > 2)
	{
		hpOffset.y = 0 - 2;
		DrawRectanglePro(hpSize, hpOffset, globalRotation, GREEN);
	}

	GameObject::Draw();
}

void Player::Update()
{
	GameObject::Update();
	
	ManageTimers();
	Input_Rotate();
	Input_Booster();
	Input_Shoot();
	ApplyVelocity();
	CollisionCheck();
}

void Player::ManageTimers()
{
	//Shoot rest timer
	if (shootRestTimer > 0)
	{
		shootRestTimer -= GetFrameTime();
		//Timeout
		if (shootRestTimer <= 0)
		{ shootRestTimer = 0; }
	}
	//Damage rest timer
	if (damageRestTimer > 0)
	{
		damageRestTimer -= GetFrameTime();
		//Timeout
		if (damageRestTimer <= 0)
		{ damageRestTimer = 0; }
	}
	//Invincible timer
	if (invTimer > 0)
	{
		invTimer -= GetFrameTime();
		//Timeout
		if (invTimer <= 0)
		{ invTimer = 0; }
	}
}


//MOVEMENT AND INPUTS

void Player::ApplyVelocity()
{
	localPosition = Vector2Add(localPosition, velocity);
	//Clamp position
	localPosition.x = Clamp(localPosition.x, -32, Game::GetInstance()->worldSize.x + 32);
	localPosition.y = Clamp(localPosition.y, -32, Game::GetInstance()->worldSize.y + 32);
}


void Player::Input_Rotate()
{
	//Get distance of the mouse from player
	float mouseDistance = Vector2Length(Vector2Subtract(Vector2Subtract(globalPosition, Game::GetInstance()->cameraPosition), GetMousePosition()));
	if (mouseDistance > 12)
	{
		//Get direction of mouse from player
		float mouseDirection = Vector2Angle(Vector2Subtract(globalPosition, Game::GetInstance()->cameraPosition), GetMousePosition());
		//Set player's rotation
		localRotation = mouseDirection;
	}
}

void Player::Input_Booster()
{
	//Player accelerate
	if (IsMouseButtonDown(key_boost) &&
		Vector2Length(Vector2Add(velocity, Vector2Scale(Vector2Rotate(Vector2Right, globalRotation), acceleration))) < maxSpeed)
	{
		velocity = Vector2Add(velocity, Vector2Scale(Vector2Rotate(Vector2Right, globalRotation), acceleration));
	}
	//Player deccelerate
	else
	{
		velocity.x = Lerp(velocity.x, 0.0f, decceleration * 0.01f);
		velocity.y = Lerp(velocity.y, 0.0f, decceleration * 0.01f);
	}
}

void Player::Input_Shoot()
{
	if (IsMouseButtonDown(key_shoot) && shootRestTimer == 0)
	{
		//Spawn bullet
		Vector2 bulletSpawnPos = Vector2Add(globalPosition, Vector2Rotate(Vector2Scale(Vector2Right, sprite->width / 2), globalRotation));
		Game::GetInstance()->InstanceObject(new Bullet(Vector2Rotate(Vector2Right, globalRotation), 4), bulletSpawnPos.x, bulletSpawnPos.y);
		//Set timer
		shootRestTimer = shootRest;
	}
}


//DAMAGE

void Player::CollisionCheck()
{

	//Check for bullet
	if (damageRestTimer == 0 && invTimer == 0 && cs->GetOverlappingColliders().size() > 0)
	{
		//Hit by bullet
		if (cs->GetOverlappingColliders()[0]->parent->name == "Bullet")
		{
			cs->GetOverlappingColliders()[0]->parent->~GameObject();
			Damage(1);
		}
		else //Hit by enemy body
		{
			Die();
		}
	}
}

void Player::Damage(int dmg)
{
	hp -= dmg;
	//Set damage rest timer
	damageRestTimer = damageRest;
	//Die when hp reaches 0
	if (hp <= 0)
	{
		Die();
	}
}

void Player::Die()
{
	//Set respawn timer
	Game::GetInstance()->playerSpawnTimer = Game::GetInstance()->playerSpawnTime;
	//Create explosion
	Game::GetInstance()->InstanceObject(new Explosion, globalPosition.x + 2.0f, globalPosition.y);
	//Destroy self
	GameObject* ptr = this;
	ptr->~GameObject();
}
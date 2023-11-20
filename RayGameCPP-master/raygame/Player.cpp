#include "Game.h"
#include "Player.h"
#include "Bullet.h"
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

	//Set sprite
	sprite = &Game::GetInstance()->spr_player;
	spriteSize = &Game::GetInstance()->rect_player;
	destination = Game::GetInstance()->rect_player;
	spriteOffset = Vector2(); spriteOffset.x = sprite->height/2; spriteOffset.y = sprite->width/2;

	//Set start position
	localPosition.x = 300; localPosition.y = 200;

	//Set movement values
	maxSpeed = 6;
	acceleration = 0.4;
	decceleration = 1;

	//Set key binds
	key_boost = MOUSE_RIGHT_BUTTON;
	key_shoot = MOUSE_LEFT_BUTTON;


	//CREATE CHILD OBJECT
	//GameObject::Start();
	//InstanceObject(new GameObject("Child0"));
	//InstanceObject(new GameObject("Child1"));

	//DESTROY SELF
	//GameObject* ptr = this;
	//ptr->~GameObject();
}

void Player::Draw()
{
	GameObject::Draw();
	//Draw at position
	destination.x = globalPosition.x; destination.y = globalPosition.y;
	//Draw player
	DrawTexturePro(*sprite, *spriteSize, destination, spriteOffset, globalRotation, WHITE);
}

void Player::Update()
{
	GameObject::Update();
	
	Input_Rotate();
	Input_Booster();
	Input_Shoot();
	ApplyVelocity();
	
}


void Player::ApplyVelocity()
{
	localPosition = Vector2Add(localPosition, velocity);
}


void Player::Input_Rotate()
{
	//Get direction of mouse from player
	float mouseDirection = Vector2Angle(globalPosition, GetMousePosition());
	//Set player's rotation
	localRotation = mouseDirection;
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
	if (IsMouseButtonPressed(key_shoot))
	{
		Game::GetInstance()->InstanceObject(new Bullet(Vector2Add(globalPosition, Vector2Rotate(Vector2Scale(Vector2Right, sprite->width / 2), globalRotation)), Vector2Rotate(Vector2Right, globalRotation)));
	}
}

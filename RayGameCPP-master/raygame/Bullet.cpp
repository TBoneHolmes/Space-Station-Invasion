#include "Bullet.h"
#include "Game.h"
#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <string>
using namespace std;


//MOVEMENT
const float speed = 700;

//Constructor
Bullet::Bullet(Vector2 direction, int layer)
{
	GameObject::name = "Bullet";
	//Set direction
	moveDirection = direction;
	//Set collisionLayer
	colLayer = layer;
	velocityPlus = Vector2Zero();

	bulletCol = WHITE;
}
Bullet::Bullet(Vector2 direction, int layer, Color col)
{
	GameObject::name = "Bullet";
	//Set direction
	moveDirection = direction;
	//Set collisionLayer
	colLayer = layer;
	bulletCol = col;
}



void Bullet::Start()
{
	GameObject::Start();

	//Set draw order
	drawOrder = 2;

	//Set sprite
	sprite = &Game::GetInstance()->spr_bullet;
	spriteSize = &Game::GetInstance()->rect_bullet;
	destination = Game::GetInstance()->rect_bullet;
	spriteOffset = Vector2(); spriteOffset.x = sprite->height / 2; spriteOffset.y = sprite->width / 2;

	//Create collision shape
	InstanceObject(new CollisionShape(6, colLayer, 0), 0, 0);
}

void Bullet::Draw()
{
	//Draw at position
	destination.x = globalPosition.x - Game::GetInstance()->cameraPosition.x; destination.y = globalPosition.y - Game::GetInstance()->cameraPosition.y;
	//Draw bullet
	DrawTexturePro(*sprite, *spriteSize, destination, spriteOffset, globalRotation, bulletCol);

	GameObject::Draw();
}

void Bullet::Update()
{
	GameObject::Update();

	ApplyVelocity();
	DestroyCheck();
}


void Bullet::ApplyVelocity()
{
	//Set velocity
	velocity = Vector2Scale(moveDirection, speed);
	velocity = Vector2Add(velocity, velocityPlus);
	//Clamp velocity
	velocity = Vector2Scale(Vector2Normalize(velocity), speed);
	//Apply velocity
	localPosition = Vector2Add(localPosition, Vector2Scale(velocity, GetFrameTime()));
}

void Bullet::DestroyCheck()
{
	//Destroy when outside of camera range
	if (!Game::GetInstance()->InCamera(globalPosition, 64))
	{
		//Destroy self
		Destroy();
	}
}
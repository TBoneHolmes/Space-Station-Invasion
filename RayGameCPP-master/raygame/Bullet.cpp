#include "Bullet.h"
#include "Game.h"
#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <string>
using namespace std;

//Constructor
Bullet::Bullet(Vector2 position, Vector2 direction)
{
	GameObject::name = "Bullet";
	//Set position
	localPosition = position;
	//Set direction
	moveDirection = direction;
}


void Bullet::Start()
{
	GameObject::Start();

	//Set sprite
	sprite = &Game::GetInstance()->spr_bullet;
	spriteSize = &Game::GetInstance()->rect_bullet;
	destination = Game::GetInstance()->rect_bullet;
	spriteOffset = Vector2(); spriteOffset.x = sprite->height / 2; spriteOffset.y = sprite->width / 2;

	//Set movement values
	speed = 10;
}

void Bullet::Draw()
{
	GameObject::Draw();
	//Draw at position
	destination.x = globalPosition.x; destination.y = globalPosition.y;
	//Draw bullet
	DrawTexturePro(*sprite, *spriteSize, destination, spriteOffset, globalRotation, WHITE);
}

void Bullet::Update()
{
	GameObject::Update();

	ApplyVelocity();
}


void Bullet::ApplyVelocity()
{
	//Set velocity
	velocity = Vector2Scale(moveDirection, speed);
	//Apply velocity
	localPosition = Vector2Add(localPosition, velocity);
}
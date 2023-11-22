#include "CollisionShape.h"
#include "Game.h"
#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <string>
using namespace std;

//Constructor
CollisionShape::CollisionShape(float rad, unsigned char layer, unsigned char mask)
{
	CollisionShape::name = "CollisionShape";
	//Set radius
	radius = rad;
	//Set collision layer and mask
	collisionLayer = layer;
	collisionMask = mask;
}
CollisionShape::CollisionShape(float rad, Vector2 position, unsigned char layer, unsigned char mask)
{
	CollisionShape::name = "CollisionShape";
	//Set radius
	radius = rad;
	//Set position
	localPosition = position;
	//Set collision layer and mask
	collisionLayer = layer;
	collisionMask = mask;
}


void CollisionShape::Start()
{
	GameObject::Start();

	//Add self to the game manager's CollisionShapes list
	Game::GetInstance()->collisionShapes.push_back(this);

	enabled = true;
}

void CollisionShape::Draw()
{
	GameObject::Draw();

	if (Game::GetInstance()->drawCollisions)
	{
		//Get color depending on "enabled"
		Color csColor;
		if (enabled)
		{ csColor.r = 94; csColor.g = 188; csColor.b = 255; csColor.a = 180; }
		else
		{ csColor.r = 255; csColor.g = 94; csColor.b = 94; csColor.a = 180; }
		//Draw collision shape
		DrawCircle(globalPosition.x, globalPosition.y, radius, csColor);

		//DEBUG - Print collisionLayer and collisionMask bits
		//cout << parent->name << endl
		//	<< "Layer: ";
		//for (int i = 0; i < 8; i++)
		//{
		//	cout << ((collisionLayer & (1 << i)) != 0);
		//}
		//cout << endl
		//	<< " Mask: ";
		//for (int i = 0; i < 8; i++)
		//{
		//	cout << ((collisionMask & (1 << i)) != 0);
		//}
		//cout << endl << endl;
	}
}

void CollisionShape::Update()
{
	GameObject::Update();

	//Check for collisions
	if (enabled)
	{
		for (int i = 0; i < Game::GetInstance()->collisionShapes.size(); i++)
		{
			CollisionShape* checkPtr = Game::GetInstance()->collisionShapes[i];
			if (checkPtr != this && checkPtr->enabled
				//Check collision
				&& CheckCollisionCircles(globalPosition, radius, checkPtr->globalPosition, checkPtr->radius)
				//Check mask
				&& CheckMask(collisionLayer, checkPtr->collisionMask) == true)
			{
				Collide(checkPtr);
			}
		}
	}
}

//CollisionShape has collided with another CollisionShape
void CollisionShape::Collide(CollisionShape* collider)
{
	cout << parent->name << " collided with: " << collider->parent->name << endl;
	//cout << collisionLayer << " layer collided with: " << collider->collisionMask << endl;
}

//Returns true if this collision shape contains the given mask int
bool CollisionShape::CheckMask(int layer, int mask)
{
	for (int i = 0; i < 8; i++)
	{
		//Compare each bit of the layer and mask
		if (((layer & (1 << i)) != 0) && ((mask & (1 << i)) != 0))
		{
			return true;
		}
	}
	return false;
}
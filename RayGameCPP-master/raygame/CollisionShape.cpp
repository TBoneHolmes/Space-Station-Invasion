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
	GameObject::name = "CollisionShape";
	//Set radius
	radius = rad;
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

	if (Game::GetInstance()->drawCollisions)
	{
		//Get color depending on "enabled"
		Color csColor;
		if (enabled)
		{ csColor.r = 94; csColor.g = 188; csColor.b = 255; csColor.a = 180; }
		else
		{ csColor.r = 255; csColor.g = 94; csColor.b = 94; csColor.a = 180; }
		//Draw collision shape
		DrawCircle(globalPosition.x - Game::GetInstance()->cameraPosition.x, globalPosition.y - Game::GetInstance()->cameraPosition.y, radius, csColor);

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

	GameObject::Draw();

	}
}

void CollisionShape::Update()
{
	GameObject::Update();
	
	//DEBUG Print collisions
	//if (GetOverlappingColliders().size() > 0)
	//{
	//	cout << parent->name << " collided with: ";
	//	for (int i = 0; i < GetOverlappingColliders().size(); i++)
	//	{
	//		cout << GetOverlappingColliders()[i]->parent->name << endl;
	//	}
	//}
}

//Returns a vector with all colliders that are overlapping with this one
vector<CollisionShape*> CollisionShape::GetOverlappingColliders()
{
	//The list that will return at the end
	vector<CollisionShape*> returnVector;
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
				&& CheckMask(checkPtr->collisionLayer, collisionMask) == true)
			{
				returnVector.push_back(checkPtr);
			}
		}
	}

	return returnVector;
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

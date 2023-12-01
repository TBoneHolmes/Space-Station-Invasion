#include "raylib.h"
#include "CollisionShape.h"
#include <string>
#include "GameObject.h"
using namespace std;

#pragma once
class Powerup : public GameObject
{

public:
	//-CONSTRUCTOR-
	Powerup();

	//-VARIABLES-
	//Sprite
	Texture2D* sprite;
	Rectangle* spriteSize;
	Rectangle destination;
	Vector2 spriteOffset;
	//Collision
	CollisionShape* cs;

	//-FUNCTIONS-
	void Start();
	void Draw();
	void Update();
	void CollisionCheck();
	void Collect();
};
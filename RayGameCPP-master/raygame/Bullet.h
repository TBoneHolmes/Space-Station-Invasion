#include "raylib.h"
#include <string>
#include "GameObject.h"
using namespace std;

#pragma once
class Bullet : public GameObject
{

public:
	//-CONSTRUCTOR-
	Bullet(Vector2);

	//-VARIABLES-
	//Sprite
	Texture2D* sprite;
	Rectangle* spriteSize;
	Rectangle destination;
	Vector2 spriteOffset;
	//Movement
	Vector2 velocity;
	Vector2 moveDirection;
	float speed;

	//-FUNCTIONS-
	void Start();
	void Draw();
	void Update();
	void DestroyCheck();
	//Movement
	void ApplyVelocity();
};
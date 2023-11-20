#include "raylib.h"
#include <string>
#include "GameObject.h"
using namespace std;

#pragma once
class Player : public GameObject
{

public:
	//-CONSTRUCTOR-
	Player();

	//-VARIABLES-
	//Sprite
	Texture2D* sprite;
	Rectangle* spriteSize;
	Rectangle destination;
	Vector2 spriteOffset;
	//Input
	int key_boost;
	int key_shoot;
	//Movement
	Vector2 velocity;
	float maxSpeed;
	float acceleration;
	float decceleration;

	//-FUNCTIONS-
	void Start();
	void Draw();
	void Update();
	void ApplyVelocity();
	//Inputs
	void Input_Rotate();
	void Input_Booster();
	void Input_Shoot();

};

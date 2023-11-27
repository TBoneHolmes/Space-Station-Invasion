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
	//Collision Shape
	CollisionShape* cs;
	//Input
	int key_boost;
	int key_shoot;
	//Movement
	Vector2 velocity;
	float maxSpeed;
	float acceleration;
	float decceleration;
	//Damage
	int hp;
	float damageRest;
	float damageRestTimer;

	//-FUNCTIONS-
	void Start();
	void Draw();
	void Update();
	void ApplyVelocity();
	//Inputs
	void Input_Rotate();
	void Input_Booster();
	void Input_Shoot();
	//Damage
	void Damage(int);
	void Die();
	void CollisionCheck();

};

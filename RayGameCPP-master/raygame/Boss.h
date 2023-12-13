#pragma once
#include "GameObject.h"
#include "CollisionShape.h"
class Boss : public GameObject
{
public:
	//-CONSTRUCTOR-
	Boss();

	//-VARIABLES-
	//Sprite
	Texture2D* sprite;
	Rectangle* spriteSize;
	Rectangle destination;
	Vector2 spriteOffset;
	//Collision Shape
	CollisionShape* cs;
	//Damage
	int hp;
	float damageRestTimer;
	//Movement
	Vector2 velocity;
	Vector2 playerTargetOffset;
	float maxSpeed;
	float shootRestTimer;

	//-FUNCTIONS-
	void Start();
	void Draw();
	void Update();
	void Destroy();
	void ManageTimers();
	//Damage
	void Damage(int);
	void Die();
	void CollisionCheck();
	//Movement
	void MoveToPoint();
	void Accelerate();
	void ApplyVelocity();
};


#pragma once
#include "GameObject.h"
#include "CollisionShape.h"
class EnemyDefault : public GameObject
{
public:
	//-CONSTRUCTOR-
	EnemyDefault();

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
	Vector2 targetPoint;
	Vector2 playerTargetOffset;
	float maxSpeed;
	//Shoot
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
	//Shoot
	void Shoot();
	//AI
	void Ai();
};


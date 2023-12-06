#pragma once
#include "GameObject.h"
#include "CollisionShape.h"
class BossShield : public GameObject
{
public:
	//-CONSTRUCTOR-
	BossShield(float);

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
	float damageRest;
	float damageRestTimer;
	//Movement
	float targetAngle;
	int targetDist;
	float speed;
	float rotationSpeed;
	//Shoot
	float shootRest;
	float shootRestTimer;
	//Score
	int killScore;

	//-FUNCTIONS-
	void Start();
	void Draw();
	void Update();
	void ManageTimers();
	//Damage
	void Damage(int);
	void Die();
	void CollisionCheck();
	//Shoot
	void Shoot();
	//Movement
	void SetPosition();
	void ApplyMovement();
};


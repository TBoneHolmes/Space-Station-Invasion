#include "raylib.h"
#include "GameObject.h"
#include "CollisionShape.h"
#include <string>
using namespace std;


#pragma once
class Base : public GameObject
{

public:
	//-CONSTRUCTOR-
	Base();

	//-VARIABLES-
	//Sprite
	Texture2D* sprite;
	Rectangle* spriteSize;
	Rectangle destination;
	Vector2 spriteOffset;
	//Collision Shape
	CollisionShape* cs;
	//Damage
	int maxHp;
	int hp;
	float damageRestTimer;
	float hitNotifyTimer;

	//-FUNCTIONS-
	void Start();
	void Draw();
	void Update();
	//Damage
	void ManageTimers();
	void CollisionCheck();
	void Damage(int);
	void Die();
};
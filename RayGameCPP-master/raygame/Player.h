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
	Texture2D* spriteBooster;
	Rectangle* spriteSize;
	Rectangle spriteBoosterSize;
	Rectangle destination;
	Rectangle destinationBooster;
	Vector2 spriteOffset;
	Vector2 spriteBoosterOffset;
	int spriteFrames;
	int frame;
	float animationTimer;
	//Collision Shape
	CollisionShape* cs;
	//Input
	int key_boost;
	int key_shoot;
	//Movement
	Vector2 velocity;
	//Damage
	int hp;
	float damageRestTimer;
	//Shoot
	float shootRest;
	float shootRestTimer;
	//Invincibility
	float invTimer;
	//Powerup
	float powerupTime;
	float powerupTimer;
	int powerupColor;
	int bulletColor;
	bool powerupColorChanged;

	//-FUNCTIONS-
	void Start();
	void Draw();
	void Update();
	void Destroy();
	void ApplyVelocity();
	void ManageTimers();
	void ManageAnimation();
	void ManagePowerup();
	//Inputs
	void Input_Rotate();
	void Input_Booster();
	void Input_Shoot();
	//Damage
	void Damage(int);
	void Die();
	void CollisionCheck();

};

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
	float animationSpeed;
	float animationTimer;
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
	int maxHp;
	int hp;
	float damageRest;
	float damageRestTimer;
	//Shoot
	float shootRest;
	float shootRestTimer;
	//Invincibility
	float invTime;
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

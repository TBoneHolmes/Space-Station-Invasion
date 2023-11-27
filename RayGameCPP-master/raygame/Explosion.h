#include "raylib.h"
#include "GameObject.h"
#include <string>
using namespace std;

//LAYERS
//1 = player
//2 = enemy
//4 = bullet


#pragma once
class Explosion : public GameObject
{

public:
	//-CONSTRUCTOR-
	Explosion();

	//-VARIABLES-
	//Sprite
	Texture2D* sprite;
	Rectangle* spriteSize;
	Rectangle destination;
	Vector2 spriteOffset;
	int spriteFrames;
	//Animation
	float animationSpeed;
	float animationTimer;
	int frame;

	//-FUNCTIONS-
	void Start();
	void Draw();
	void Update();
};
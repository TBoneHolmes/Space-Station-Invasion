#include "raylib.h"
#include "GameObject.h"
#include <string>
using namespace std;


#pragma once
class Explosion : public GameObject
{

public:
	//-CONSTRUCTOR-
	Explosion();
	Explosion(float);

	//-VARIABLES-
	//Sprite
	Texture2D* sprite;
	Rectangle spriteSize;
	Rectangle destination;
	Vector2 spriteOffset;
	int spriteFrames;
	//Animation
	float animationTimer;
	int frame;

	//-FUNCTIONS-
	void Start();
	void Draw();
	void Update();
	void Destroy();
	//Animation
	void ManageAnimation();
};
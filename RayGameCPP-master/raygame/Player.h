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
	Vector2 input;

	//-FUNCTIONS-
	void Start();
	void Draw();
	void Update();
	void Move();
	//Inputs
	Vector2 GetMouseRelative();

};

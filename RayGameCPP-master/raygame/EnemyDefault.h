#pragma once
#include "GameObject.h"
class EnemyDefault : public GameObject
{
public:
	//-CONSTRUCTOR-
	EnemyDefault(Vector2);

	//-VARIABLES-
	//Sprite
	Texture2D* sprite;
	Rectangle* spriteSize;
	Rectangle destination;
	Vector2 spriteOffset;

	//-FUNCTIONS-
	void Start();
	void Draw();
	void Update();
};


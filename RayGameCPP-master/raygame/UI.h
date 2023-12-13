#include "raylib.h"
#include "GameObject.h"
#include <string>
using namespace std;


#pragma once
class UI : public GameObject
{

public:
	//-CONSTRUCTOR-
	UI();

	//-VARIABLES-
	Vector2 offset;
	Vector2 mapSize;

	//-FUNCTIONS-
	void Start();
	void Draw();
	void Update();
};
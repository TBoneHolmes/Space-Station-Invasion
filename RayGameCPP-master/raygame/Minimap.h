#include "raylib.h"
#include "GameObject.h"
#include <string>
using namespace std;


#pragma once
class Minimap : public GameObject
{

public:
	//-CONSTRUCTOR-
	Minimap();

	//-VARIABLES-
	Vector2 offset;
	Vector2 mapSize;
	float mapScale;

	//-FUNCTIONS-
	void Start();
	void Draw();
	void Update();
};
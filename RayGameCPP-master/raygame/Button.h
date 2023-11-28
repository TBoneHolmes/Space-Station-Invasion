#include "raylib.h"
#include "GameObject.h"
#include <string>
using namespace std;


#pragma once
class Button : public GameObject
{

public:
	//-CONSTRUCTOR-
	Button(const char*, int, int);

	//-VARIABLES-
	//Draw
	Vector2 size;
	//Interaction
	int mouseInteract;
	char* text;

	//-FUNCTIONS-
	void Start();
	void Draw();
	void Update();
	void DetectMouse();
	void Clicked();
};
#include "raylib.h"
#include "GameObject.h"
#include <string>
using namespace std;


#pragma once
class Button : public GameObject
{

public:
	//-CONSTRUCTOR-
	Button(string, int, int);

	//-VARIABLES-
	//Draw
	Vector2 size;
	//Interaction
	int mouseInteract;
	string text;

	//-FUNCTIONS-
	void Start();
	void Draw();
	void Update();
	void DetectMouse();
	void Clicked();
};
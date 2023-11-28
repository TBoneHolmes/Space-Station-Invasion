#include "Button.h"
#include "Game.h"
#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <string>
using namespace std;

//Constructor
Button::Button(string txt, int w, int h)
{
	GameObject::name = "Button";

	text = txt;
	size.x = w;
	size.y = h;
}


void Button::Start()
{
	GameObject::Start();

	//Set draw order
	drawOrder = 0;

	mouseInteract = 0; //0 = none, 1 = hovered, 2 = pressed
}

void Button::Draw()
{
	//Get color
	int colShade = 200;
	if (mouseInteract == 2)
	{ colShade = 130; }
	else if (mouseInteract == 1)
	{ colShade = 170;}
	Color buttonCol; buttonCol.r = colShade; buttonCol.g = colShade; buttonCol.b = colShade;
	//Draw outline
	//Draw button
	DrawRectangle(globalPosition.x - (size.x / 2), globalPosition.y - (size.y / 2), size.x, size.y, buttonCol);
	//Draw text

	GameObject::Draw();
}

void Button::Update()
{
	GameObject::Update();
}

void Button::DetectMouse()
{
	if (GetMousePosition().x > globalPosition.x - (size.x / 2) && GetMousePosition().x < globalPosition.x + (size.x / 2)
		&& GetMousePosition().y > globalPosition.y - (size.y / 2) && GetMousePosition().y < globalPosition.y + (size.y / 2))
	{

	}
}

void Button::Clicked()
{

}
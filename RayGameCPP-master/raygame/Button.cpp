#include "Button.h"
#include "Game.h"
#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <string>
using namespace std;

//Constructor
Button::Button(const char* txt, int w, int h)
{
	GameObject::name = "Button";

	text = (char*)txt;
	size.x = w;
	size.y = h;
	fontScale = 1;
}
Button::Button(const char* txt, float f, int w, int h)
{
	GameObject::name = "Button";

	text = (char*)txt;
	size.x = w;
	size.y = h;
	fontScale = f;
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
	{ colShade = 110; }
	else if (mouseInteract == 1)
	{ colShade = 160;}
	//Button colow
	Color buttonCol; buttonCol.r = colShade; buttonCol.g = colShade; buttonCol.b = colShade;
	//Outline color
	int outlineShade = (colShade - 70 < 0) ? 0 : colShade - 70;
	Color outlineCol; outlineCol.r = outlineShade; outlineCol.g = outlineShade; outlineCol.b = outlineShade;
	//Font color
	int fontShade = (colShade - 140 < 0) ? 0 : colShade - 140;
	Color fontCol; fontCol.r = fontShade; fontCol.g = fontShade; fontCol.b = fontShade;
	//Draw outline
	int outlineSize = (size.x + size.y) / 64;
	DrawRectangle(globalPosition.x - (size.x / 2) - outlineSize, globalPosition.y - (size.y / 2) - outlineSize, size.x + (outlineSize * 2), size.y + (outlineSize * 2), outlineCol);
	//Draw button
	DrawRectangle(globalPosition.x - (size.x / 2), globalPosition.y - (size.y / 2), size.x, size.y, buttonCol);
	//Draw text
	int fontSize = (size.y / 2) * fontScale;
	int textWidth = MeasureText(text, fontSize);
	DrawText(text, globalPosition.x - (textWidth / 2), globalPosition.y - (fontSize / 2.5), fontSize, fontCol);
	
	GameObject::Draw();
}

void Button::Update()
{
	GameObject::Update();

	DetectMouse();
}

void Button::DetectMouse()
{
	//Mouse is inside button
	if (GetMousePosition().x > globalPosition.x - (size.x / 2) && GetMousePosition().x < globalPosition.x + (size.x / 2)
		&& GetMousePosition().y > globalPosition.y - (size.y / 2) && GetMousePosition().y < globalPosition.y + (size.y / 2))
	{
		//Mouse pressed
		if (IsMouseButtonDown(0))
		{ mouseInteract = 2; }
		else //Mouse hovered
		{ mouseInteract = 1; }

		//Mouse released
		if (IsMouseButtonReleased(0))
		{ Clicked(); }
	}
	else
	{
		mouseInteract = 0;
	}
}

void Button::Clicked()
{
	if (text == "RESTART" || text == "START")
	{
		Game::GetInstance()->StartGame();
	}
	else if (text == "QUIT")
	{
		CloseWindow();
	}
	else if (text == "FULLSCREEN")
	{
		ToggleFullscreen();
	}
}
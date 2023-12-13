#include "ScoreNotifier.h"
#include "Game.h"
#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <string>
using namespace std;

//Constructor
ScoreNotifier::ScoreNotifier(int s)
{
	GameObject::name = "ScoreNotifier";
	score = s;
}


void ScoreNotifier::Start()
{
	GameObject::Start();

	drawOrder = 4;

	fontSize = 42;
	localRotation = 20;
	//Color
	alpha = 255;
}

void ScoreNotifier::Draw()
{
	if (alpha > 0)
	{
		//Set text color
		Color textCol; textCol.r = 50; textCol.g = 255; textCol.b = 50;
		textCol.a = alpha;

		
		char* text = (char*)"%00i";
		//Set text position
		Vector2 posPlus; posPlus.x = (fontSize / 2); posPlus.y = fontSize / 2;
		if (score >= 10 && score < 100)
		{ posPlus.x += fontSize / 2; }
		else if (score >= 100)
		{ posPlus.x += fontSize; }
		Vector2 textPos; textPos.x = (globalPosition.x - Game::GetInstance()->cameraPosition.x) - (posPlus.x); textPos.y = (globalPosition.y - Game::GetInstance()->cameraPosition.y) - posPlus.y;
		//Draw text
		DrawTextEx(Game::GetInstance()->fnt_gameover, TextFormat(text, score), textPos, fontSize, -2, textCol);
		
		GameObject::Draw();
	}
}

void ScoreNotifier::Update()
{
	GameObject::Update();

	
	
	//Drift upward
	localPosition.y -= 60 * GetFrameTime();
	
	
	//Shrink text size
	if (fontSize > 16)
	{ fontSize = Lerp(fontSize, 16, 10 * GetFrameTime()); }
	
	
	//Fade text out
	if (fontSize <= 18) { alpha -= 2000 * GetFrameTime(); }
	//Destroy
	if (alpha <= 0)
	{
		Destroy();
	}
}
#include "raylib.h"
#include "GameObject.h"
#include <string>
using namespace std;


#pragma once
class ScoreNotifier : public GameObject
{

public:
	//-CONSTRUCTOR-
	ScoreNotifier(int);

	//-VARIABLES-
	int score;
	float fontSize;
	int alpha;

	//-FUNCTIONS-
	void Start();
	void Draw();
	void Update();
};
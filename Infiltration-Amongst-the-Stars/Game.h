#include "GameObject.h"
#include "vector"
#include <list>
using namespace std;

#pragma once
class Game
{

public:
	//Constructor
	Game();

	//Variables
	vector<GameObject*> objects;

	//Functions
	void Update();

};


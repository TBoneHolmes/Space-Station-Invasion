#include "GameObject.h"
#include "Player.h"
#include <vector>
#include <string>
using namespace std;

#pragma once
class Game
{

protected:
	//Constructor
	Game();
	static Game* instance;

public:

	static Game* GetInstance();
	//Deconstructor
	~Game();

	//Variables
	vector<GameObject*> scene;
	int test;

	//Functions
	void Start();
	void Update();

	void operator = (const Game&) = delete;
	Game(const Game& other) = delete;

};


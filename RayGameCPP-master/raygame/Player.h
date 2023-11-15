#include <string>
#include "GameObject.h"
using namespace std;

#pragma once
class Player : public GameObject
{

public:
	//Constructor
	Player();

	//Variables

	//Functions
	void Start();
	void Draw();
	void Update();

};

#include <string>
using namespace std;

#pragma once
class GameObject
{

public:
	//Constructor
	GameObject();
	GameObject(string);

	//Variables
	//string name;
	string name;

	//Functions
	void virtual Update();

};


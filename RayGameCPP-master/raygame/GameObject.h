#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <string>
using namespace std;

#pragma once
class GameObject
{

public:
	//Constructor
	GameObject();
	GameObject(string);
	//Deconstructor
	~GameObject();

	//Variables
	string name;
	Vector2 localPosition;
	Vector2 globalPosition;
	float localRotation;
	float globalRotation;
	GameObject* parent;
	vector<GameObject*> children;

	//Functions
	void virtual Start();
	void virtual Draw();
	void virtual Update();
	void virtual InstanceObject(GameObject*);


};


#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <string>
using namespace std;

//DRAW ORDER
//0 - Base
//1 - Asteroids
//2 - Enemies, bullets
//3 - Player
//4 - UI

#pragma once
class GameObject
{

public:
	//Constructor
	GameObject();
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
	Vector2 Vector2Right;
	int drawOrder;

	//Functions
	void virtual Start();
	void virtual Draw();
	void virtual Update();
	void virtual OnCollision(GameObject*);
	void virtual InstanceObject(GameObject*, int, int);


};


#include "raylib.h"
#include "GameObject.h"
#include <string>
using namespace std;

//LAYERS
//1 = player
//2 = enemy
//4 = player bullet
//8 = enemy bullet


#pragma once
class CollisionShape : public GameObject
{

public:
	//-CONSTRUCTOR-
	CollisionShape(float, unsigned char, unsigned char);

	//-VARIABLES-
	float radius;
	bool enabled;
	unsigned char collisionLayer;
	unsigned char collisionMask;

	//-FUNCTIONS-
	void Start();
	void Draw();
	void Update();
	bool CheckMask(int, int);
	vector<CollisionShape*> GetOverlappingColliders();
};
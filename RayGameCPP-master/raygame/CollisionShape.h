#include "raylib.h"
#include "GameObject.h"
#include <string>
#include <cstddef>
#include <cstdint>
using namespace std;

//LAYERS
//1 = player
//2 = enemy
//4 = bullet


#pragma once
class CollisionShape : public GameObject
{

public:
	//-CONSTRUCTOR-
	CollisionShape(float, unsigned char, unsigned char);
	CollisionShape(float, Vector2, unsigned char, unsigned char);

	//-VARIABLES-
	float radius;
	bool enabled;
	unsigned char collisionLayer;
	unsigned char collisionMask;

	//-FUNCTIONS-
	void Start();
	void Draw();
	void Update();
	void Collide(CollisionShape*);
	bool CheckMask(int, int);
};
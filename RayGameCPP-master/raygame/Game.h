#include "GameObject.h"
#include "CollisionShape.h"
#include "Player.h"
#include "Bullet.h"
#include "EnemyDefault.h"
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
	vector<CollisionShape*> collisionShapes;
	int test;
	//Game
	Vector2 worldSize;
	//Camera
	Vector2 cameraPosition;
	Vector2 cameraSize;
	GameObject* cameraOwner;
	//Textures
	Texture2D spr_background;
	Rectangle rect_background;
	Texture2D spr_player;
	Rectangle rect_player;
	Texture2D spr_bullet;
	Rectangle rect_bullet;
	//For debugging
	bool drawCollisions;

	//Functions
	void Start();
	void Draw();
	void Update();
	void InstanceObject(GameObject*, int, int);

	//Singleton operator override
	void operator = (const Game&) = delete;
	Game(const Game& other) = delete;

};


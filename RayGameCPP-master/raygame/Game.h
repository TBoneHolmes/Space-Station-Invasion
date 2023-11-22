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
	//Textures
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
	void InstanceObject(GameObject*);

	//Singleton operator override
	void operator = (const Game&) = delete;
	Game(const Game& other) = delete;

};


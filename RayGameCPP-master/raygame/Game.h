#include "GameObject.h"
#include "CollisionShape.h"
#include "Player.h"
#include "Bullet.h"
#include "EnemyDefault.h"
#include "Explosion.h"
#include "Minimap.h"
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
	vector<GameObject*> enemies;
	Player* player;
	Minimap* minimap;
	//Game
	Vector2 worldSize;
	Vector2 worldTileSize;
	Vector2 center; //Center of the world
	Vector2 playerSpawn;
	//Camera
	Vector2 cameraPosition;
	Vector2 cameraSize;
	GameObject* cameraOwner;
	//Enemy Spawn
	float enemySpawnTime;
	float enemySpawnTimer;
	//Player spawn
	float playerSpawnTime;
	float playerSpawnTimer;
	//Stats
	int score;
	//Textures
	Texture2D spr_background;
	Rectangle rect_background;
	Texture2D spr_player;
	Rectangle rect_player;
	Texture2D spr_bullet;
	Rectangle rect_bullet;
	Texture2D spr_enemyDefault;
	Rectangle rect_enemyDefault;
	Texture2D spr_explosion;
	Rectangle rect_explosion;
	int frames_explosion;
	//For debugging
	bool drawCollisions;

	//Functions
	void Start();
	void Draw();
	void Update();
	void ManageTimers();
	void CameraPosition();
	//Spawning
	void InstanceObject(GameObject*, int, int);
	void SpawnEnemy();
	void SpawnPlayer();

	//Singleton operator override
	void operator = (const Game&) = delete;
	Game(const Game& other) = delete;

};


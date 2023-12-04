#include "GameObject.h"
#include "CollisionShape.h"
class Boss : public GameObject
{
public:
	//-CONSTRUCTOR-
	Boss();

	//-VARIABLES-
	//Sprite
	Texture2D* sprite;
	Rectangle* spriteSize;
	Rectangle destination;
	Vector2 spriteOffset;
	//Collision Shape
	CollisionShape* cs;
	//Damage
	int hp;
	float damageRest;
	float damageRestTimer;
	//Movement
	Vector2 velocity;
	Vector2 targetPoint;
	Vector2 playerTargetOffset;
	float maxSpeed;
	float acceleration;
	//Shoot
	float shootRest;
	float shootRestTimer;
	//Score
	int killScore;

	//-FUNCTIONS-
	void Start();
	void Draw();
	void Update();
	void ManageTimers();
	//Damage
	void Damage(int);
	void Die();
	void CollisionCheck();
	//Movement
	void MoveToPoint();
	void Accelerate();
	void ApplyVelocity();
	//Shoot
	void Shoot();
	//AI
	void Ai();
};


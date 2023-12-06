#include "Game.h"
#include "Player.h"
#include "Bullet.h"
#include "CollisionShape.h"
#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <string>
using namespace std;

//Constructor
Player::Player()
{
	GameObject::name = "Player";
}


void Player::Start()
{
	GameObject::Start();

	//Set player reference
	Game::GetInstance()->player = this;
	//Set camera
	Game::GetInstance()->cameraOwner = this;

	//Set draw order
	drawOrder = 3;

	//Set sprite
	sprite = &Game::GetInstance()->spr_player;
	spriteBooster = &Game::GetInstance()->spr_playerBooster;
	spriteSize = &Game::GetInstance()->rect_player;
	spriteBoosterSize = Game::GetInstance()->rect_playerBooster;
	destination = Game::GetInstance()->rect_player;
	destinationBooster = Game::GetInstance()->rect_playerBooster;
	spriteOffset = Vector2(); spriteOffset.x = sprite->width / 2; spriteOffset.y = sprite->height / 2;
	spriteBoosterOffset = Vector2(); spriteBoosterOffset.x = spriteBooster->width; spriteBoosterOffset.y = spriteBooster->height / 2;

	//Animation
	spriteFrames = Game::GetInstance()->frames_playerBooster;
	frame = 0;
	animationSpeed = 15;
	animationTimer = 1;

	//Create collision shape
	InstanceObject(new CollisionShape(12, 1, 2 + 8), -4.0, 0);
	//Cache collision shape
	cs = (CollisionShape*)children.back();


	//Set movement values
	maxSpeed = 448;
	acceleration = 16;
	decceleration = 1;

	//Set key binds
	key_boost = MOUSE_RIGHT_BUTTON;
	key_shoot = MOUSE_LEFT_BUTTON;

	//Set HP
	maxHp = 3;
	hp = maxHp;
	damageRest = 0.1;

	//Set shoot timer
	shootRest = 0.3;
	shootRestTimer = 0;
	//Set invincible timer
	invTime = 1;
	invTimer = invTime;
	//Set powerup timer
	powerupTime = 20;
	powerupTimer = 0;
	powerupColor = 0;
	powerupColorChanged = false;


	//DESTROY SELF
	//GameObject* ptr = this;
	//ptr->~GameObject();
}

void Player::Draw()
{
	//Draw at position
	destination.x = globalPosition.x - Game::GetInstance()->cameraPosition.x; destination.y = globalPosition.y - Game::GetInstance()->cameraPosition.y;
	destinationBooster.x = globalPosition.x - Game::GetInstance()->cameraPosition.x; destinationBooster.y = globalPosition.y - Game::GetInstance()->cameraPosition.y;
	//Set draw color
	Color drawCol = WHITE;
	//Repsawn invinciblility color
	if (invTimer > 0 && powerupTimer == 0)
	{
		drawCol.a = 180;
	}
	//Damaged color
	else if (damageRestTimer > 0)
	{
		drawCol = RED;
	}
	//Powerup color
	else if (powerupTimer > 0)
	{
		//Only show powerup color every 0.2 seconds (to create a flashing effect)
		if (((((float)floor(powerupTimer) - powerupTimer) / 0.2) - (float)floor((((float)floor(powerupTimer) - powerupTimer) / 0.2))) > 0.5)
		{
			if (powerupColor == 0)
			{ drawCol = RED; }
			else if (powerupColor == 1)
			{ drawCol = ORANGE; }
			else if (powerupColor == 2)
			{ drawCol = YELLOW; }
			else if (powerupColor == 3)
			{ drawCol = GREEN; }
			else if (powerupColor == 4)
			{ drawCol = BLUE; }
			else if (powerupColor == 5)
			{ drawCol = VIOLET; }
			if (!powerupColorChanged)
			{
				powerupColorChanged = true;
				//Increase color selection
				powerupColor = (powerupColor < 5) ? powerupColor + 1 : 0;
			}
		}
		else
		{
			powerupColorChanged = false;
		}


	}

	//Draw booster
	if (IsMouseButtonDown(key_boost))
	{
		//Set sprite index
		spriteBoosterSize.x = (spriteBooster->width / spriteFrames) * frame;
		DrawTexturePro(*spriteBooster, spriteBoosterSize, destinationBooster, spriteBoosterOffset, globalRotation, WHITE);
	}
	//Draw player
	DrawTexturePro(*sprite, *spriteSize, destination, spriteOffset, globalRotation, drawCol);

	//Draw HP
	Rectangle hpSize; hpSize.x = globalPosition.x - Game::GetInstance()->cameraPosition.x; hpSize.y = globalPosition.y - Game::GetInstance()->cameraPosition.y; hpSize.width = 16 / maxHp; hpSize.height = sprite->height / maxHp;
	Vector2 hpOffset; hpOffset.x = sprite->width / 2; hpOffset.y = hpSize.height + 2;
	if (hp > 1)
	{
		DrawRectanglePro(hpSize, hpOffset, globalRotation, GREEN);
	} if (hp > 2)
	{
		hpOffset.y = 0 - 2;
		DrawRectanglePro(hpSize, hpOffset, globalRotation, GREEN);
	}

	GameObject::Draw();
}

void Player::Update()
{
	GameObject::Update();
	
	ManageTimers();
	ManageAnimation();
	ManagePowerup();
	Input_Rotate();
	Input_Booster();
	Input_Shoot();
	ApplyVelocity();
	CollisionCheck();
}

void Player::ManageTimers()
{
	//Shoot rest timer
	if (shootRestTimer > 0)
	{
		shootRestTimer -= GetFrameTime();
		//Timeout
		if (shootRestTimer <= 0)
		{ shootRestTimer = 0; }
	}
	//Damage rest timer
	if (damageRestTimer > 0)
	{
		damageRestTimer -= GetFrameTime();
		//Timeout
		if (damageRestTimer <= 0)
		{ damageRestTimer = 0; }
	}
	//Invincible timer
	if (invTimer > 0)
	{
		invTimer -= GetFrameTime();
		//Timeout
		if (invTimer <= 0)
		{ invTimer = 0; }
	}
	//Powerup timer
	if (powerupTimer > 0)
	{
		powerupTimer -= GetFrameTime();
		//Timeout
		if (powerupTimer <= 0)
		{ powerupTimer = 0; }
	}
}

void Player::ManageAnimation()
{
	if (IsMouseButtonDown(key_boost))
	{
		//Tick down timer
		animationTimer -= GetFrameTime() * animationSpeed;
		//Timeout
		if (animationTimer <= 0)
		{
			animationTimer = 1;
			frame += 1;
			//Destroy self if out of frame range
			if (frame >= spriteFrames)
			{
				frame = 0;
			}
		}
	}
}

void Player::ManagePowerup()
{
	if (powerupTimer > 0)
	{ shootRest = 0.15; }
	else
	{ shootRest = 0.3; }
}


//MOVEMENT AND INPUTS

void Player::ApplyVelocity()
{
	localPosition = Vector2Add(localPosition, Vector2Scale(velocity, GetFrameTime()));
	//Clamp position
	localPosition.x = Clamp(localPosition.x, -32, Game::GetInstance()->worldSize.x + 32);
	localPosition.y = Clamp(localPosition.y, -32, Game::GetInstance()->worldSize.y + 32);
}


void Player::Input_Rotate()
{
	//Get distance of the mouse from player
	float mouseDistance = Vector2Length(Vector2Subtract(Vector2Subtract(globalPosition, Game::GetInstance()->cameraPosition), GetMousePosition()));
	if (mouseDistance > 12)
	{
		//Get direction of mouse from player
		float mouseDirection = Vector2Angle(Vector2Subtract(globalPosition, Game::GetInstance()->cameraPosition), GetMousePosition());
		//Set player's rotation
		localRotation = mouseDirection;
	}
}

void Player::Input_Booster()
{
	//Player accelerate
	if (IsMouseButtonDown(key_boost) &&
		Vector2Length(Vector2Add(velocity, Vector2Scale(Vector2Rotate(Vector2Right, globalRotation), acceleration))) < maxSpeed)
	{
		//Play sfx
		if (!IsSoundPlaying(Game::GetInstance()->sfx_boostPlayer))
		{ PlaySound(Game::GetInstance()->sfx_boostPlayer); }
		//Set velocity
		velocity = Vector2Add(velocity, Vector2Scale(Vector2Rotate(Vector2Right, globalRotation), acceleration));
	}
	//Player deccelerate
	else
	{
		//Set velocity
		velocity.x = Lerp(velocity.x, 0.0f, decceleration * 0.01f);
		velocity.y = Lerp(velocity.y, 0.0f, decceleration * 0.01f);
	}
}

void Player::Input_Shoot()
{
	if (IsMouseButtonDown(key_shoot) && shootRestTimer == 0)
	{
		//Set bullet spawn position at the front of the player
		Vector2 bulletSpawnPos = Vector2Add(globalPosition, Vector2Rotate(Vector2Scale(Vector2Right, sprite->width / 2), globalRotation));
		Color newCol = WHITE;
		/* //ENABLE FOR RAINBOW BULLETS WHEN POWERED UP
		if (powerupTimer > 0)
		{
			//Set bullet color
			if (bulletColor == 0)
			{ newCol = RED; }
			else if (bulletColor == 1)
			{ newCol = ORANGE; }
			else if (bulletColor == 2)
			{ newCol = YELLOW; }
			else if (bulletColor == 3)
			{ newCol = GREEN; }
			else if (bulletColor == 4)
			{ newCol = BLUE; }
			else if (bulletColor == 5)
			{ newCol = VIOLET; }
			bulletColor = (bulletColor < 5) ? bulletColor + 1 : 0;
		}*/
		Game::GetInstance()->InstanceObject(new Bullet(Vector2Rotate(Vector2Right, globalRotation), 4, newCol), bulletSpawnPos.x, bulletSpawnPos.y);
		//Set timer
		shootRestTimer = shootRest;
		//Play sfx
		PlaySound(Game::GetInstance()->sfx_shootPlayer);
	}
}


//DAMAGE

void Player::CollisionCheck()
{

	//Check for bullet
	if (damageRestTimer == 0 && invTimer == 0 && cs->GetOverlappingColliders().size() > 0)
	{
		//Hit by bullet
		if (cs->GetOverlappingColliders()[0]->parent->name == "Bullet")
		{
			delete cs->GetOverlappingColliders()[0]->parent;
			Damage(1);
		}
		else //Hit by enemy body
		{
			Die();
		}
	}
}

void Player::Damage(int dmg)
{
	hp -= dmg;
	//Set damage rest timer
	damageRestTimer = damageRest;
	//Die when hp reaches 0
	if (hp <= 0)
	{ Die(); }
	else
	{
		Game::GetInstance()->screenshake = 10;
		PlaySound(Game::GetInstance()->sfx_hitPlayer);
	}
}


void Player::Die()
{
	Game::GetInstance()->screenshake = 20;
	Game::GetInstance()->freeze = 0.1;
	PlaySound(Game::GetInstance()->sfx_explodePlayer);
	//Set respawn timer
	Game::GetInstance()->playerSpawnTimer = Game::GetInstance()->playerSpawnTime;
	//Create explosion
	Game::GetInstance()->InstanceObject(new Explosion, globalPosition.x + 2.0f, globalPosition.y);
	//Destroy self
	Destroy();
}

void Player::Destroy()
{
	GameObject::Destroy();

	Game::GetInstance()->player = nullptr;
	Game::GetInstance()->cameraOwner = nullptr;
	//delete this;
}
#include "Game.h"
#include "Player.h"
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
	//Get sprite
	sprite = &Game::GetInstance()->spr_player;
	spriteSize = &Game::GetInstance()->rect_player;
	destination = Game::GetInstance()->rect_player;
	spriteOffset = Vector2(); spriteOffset.x = 0; spriteOffset.y = 0;

	localPosition.x = 300; localPosition.y = 200;


	//CREATE CHILD OBJECT
	//GameObject::Start();
	//InstanceObject(new GameObject("Child0"));
	//InstanceObject(new GameObject("Child1"));

	//DESTROY SELF
	//GameObject* ptr = this;
	//ptr->~GameObject();
}

void Player::Draw()
{
	GameObject::Draw();
	//Draw at position
	destination.x = globalPosition.x; destination.y = globalPosition.y;
	//Draw player
	DrawTexturePro(*sprite, *spriteSize, destination, spriteOffset, globalRotation, WHITE);
}

void Player::Update()
{
	GameObject::Update();
	
	Vector2 wtf = Vector2(); wtf.x = GetMouseRelative().x; wtf.y = GetMouseRelative().y;
	cout << wtf.x << ", " << wtf.y << endl;
}

//Returns a normalized vector2 of the mouse position relative to the player
Vector2 GetMouseRelative()
{
	Vector2 returnVec = Vector2(); returnVec.x = 1; returnVec.y = 8;
	return returnVec;
}

void Move()
{

}
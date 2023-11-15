#include "Player.h"
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
}

void Player::Draw()
{
	GameObject::Draw();
}

void Player::Update()
{
	GameObject::Update();
}
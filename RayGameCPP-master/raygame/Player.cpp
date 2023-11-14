#include "Player.h"
#include <iostream>
#include <string>
using namespace std;

//Constructor
Player::Player()
{
	GameObject::name = "Player";
}



void Player::Update()
{
	GameObject go;
	go.Update();
	//cout << name << endl;
}
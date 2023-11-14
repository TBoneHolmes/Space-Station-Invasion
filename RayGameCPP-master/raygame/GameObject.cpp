#include "GameObject.h"
#include <iostream>
#include <string>
using namespace std;

//Constructor
GameObject::GameObject()
{
	GameObject::name = "Game Object";
}
GameObject::GameObject(string NAME)
{
	GameObject::name = NAME;
}




void GameObject::Update()
{
	cout << name << endl;
}
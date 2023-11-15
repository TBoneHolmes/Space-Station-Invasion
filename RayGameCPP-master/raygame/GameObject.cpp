#include "GameObject.h"
#include "Game.h"
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
//Deconstructor
GameObject::~GameObject()
{
	//Delete all children
	auto childIter = children.begin();
	for (int i = 0; i < children.size(); i++)
	{
		children[i]->~GameObject();
	}

	GameObject* ptr = this;
	//Remove self from the game's 'scene' list
	if (parent == nullptr)
	{
		auto iter = Game::GetInstance()->scene.begin();
		for (int i = 0; i < Game::GetInstance()->scene.size(); i++)
		{
			if (Game::GetInstance()->scene[i] == ptr) //The position of self was found in the list
			{
				Game::GetInstance()->scene.erase(iter);
				break;
			}
			iter++;
		}
	}
	else { //Remove self from parent's 'children' list
		auto iter = parent->children.begin();
		for (int i = 0; i < parent->children.size(); i++)
		{
			if (parent->children[i] == ptr) //The position of self was found in the list
			{
				parent->children.erase(iter);
				break;
			}
			iter++;
		}
	}
}


void GameObject::Start()
{
}

void GameObject::Draw()
{

}

void GameObject::Update()
{
	Draw();

	cout << name << endl;
}
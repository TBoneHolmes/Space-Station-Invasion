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
	//Update position and rotation
	if (parent == nullptr) //Object is a root object
	{
		globalPosition = localPosition;
		globalRotation = localRotation;
	}
	else { //Object is child of another object
		globalPosition.x = parent->globalPosition.x + localPosition.x; globalPosition.y = parent->globalPosition.y + localPosition.y;
		globalRotation = parent->globalRotation + localRotation;
	}


	//Draw
	Draw();


	//Update child obejcts
	for (GameObject* obj : children)
	{
		obj->Update();
	}

	//DEBUG
	//string testStr = name;
	//if (parent != nullptr)
	//{ testStr += " : Parent-" + parent->name; }
	//if (children.size() > 0)
	//{
	//	testStr += " : Children-[";
	//	for (int i = 0; i < children.size(); i++)
	//	{
	//		testStr += children[i]->name + ", ";
	//	}
	//	testStr += "]";
	//}
	//cout << testStr << endl;
}


//Create an object as a child of this
void GameObject::InstanceObject(GameObject* newObj)
{
	GameObject* ptr = this;
	children.push_back(newObj);
	newObj->parent = ptr; //Set parent
	//Call the new object's start function
	newObj->Start();
}

#include "GameObject.h"
#include "CollisionShape.h"
#include "Game.h"
#include <iostream>
#include <string>
using namespace std;

//Constructor
GameObject::GameObject()
{
	GameObject::name = "Game Object";
	drawOrder = 0; //The order that this object is drawn in
	pauseIgnore = false; //If true, this object still updates while game is paused
	gameoverIgnore = false; //If true, this object still updates when gameover is true
	parent = nullptr;
}

//Deconstructor
GameObject::~GameObject()
{

	//Delete all children
	if (children.size() > 0)
	{
		auto childIter = children.begin();
		for (int i = 0; i < children.size(); i++)
		{
			//children[i]->~GameObject();
			children[i]->Destroy();
		}
	}
	cout << name << endl;
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
	else if (!parent->destroyed) { //Remove self from parent's 'children' list
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
	Vector2Right.x = 1.0f; Vector2Right.y = 0.0f;
	destroyed = false;
}

void GameObject::Draw()
{
	if (!destroyed)
	{
		//Draw child objects
		for (GameObject* obj : children)
		{
			obj->Draw();
		}
	}
}

void GameObject::Update()
{
	if (!destroyed)
	{
		//Destroy on gameover
		if (Game::GetInstance()->gameover && name != "Button")
		{
			Destroy();
		}

		//Update position and rotation
		if (parent == nullptr) //Object is a root object
		{
			globalPosition = localPosition;
			globalRotation = localRotation;
		}
		else { //Object is child of another object
			globalPosition = Vector2Add(parent->globalPosition, Vector2Rotate(localPosition, parent->globalRotation));//parent->globalPosition.x + localPosition.x; globalPosition.y = parent->globalPosition.y + localPosition.y;
			globalRotation = parent->globalRotation + localRotation;
		}
		//Clamp rotation
		if (localRotation >= 360) { localRotation -= 360; }
		else if (localRotation < 0) { localRotation += 360; }

		//Draw
		//Draw();

		//Update child obejcts
		for (GameObject* obj : children)
		{
			obj->Update();
		}
	}
}

//This object collided with another object
void GameObject::OnCollision(GameObject* collider)
{
	
}


//Create an object as a child of this
void GameObject::InstanceObject(GameObject* newObj, int posX, int posY)
{
	GameObject* ptr = this;
	//Add object to children list
	children.push_back(newObj);
	//Set object parent
	newObj->parent = ptr;
	//Set object position
	newObj->localPosition.x = posX;
	newObj->localPosition.y = posY;
	//Call the new object's start function
	newObj->Start();
}

void GameObject::Destroy()
{
	Game::GetInstance()->garbageCollection.push_back(this);
	destroyed = true;
}
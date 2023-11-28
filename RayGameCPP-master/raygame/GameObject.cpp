
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
	//Remove self from game's collisionShapes list
	if (name == "CollisionShape")
	{
		auto iter = Game::GetInstance()->collisionShapes.begin();
		for (int i = 0; i < Game::GetInstance()->collisionShapes.size(); i++)
		{
			if (Game::GetInstance()->collisionShapes[i] == ptr)
			{
				Game::GetInstance()->collisionShapes.erase(iter);
				break;
			}
			iter++;
		}
	}
	//Remove self from game's enemies list
	else if (name == "EnemyDefault")
	{
		auto iter = Game::GetInstance()->enemies.begin();
		for (int i = 0; i < Game::GetInstance()->enemies.size(); i++)
		{
			if (Game::GetInstance()->enemies[i] == ptr)
			{
				Game::GetInstance()->enemies.erase(iter);
				break;
			}
			iter++;
		}
	}
	//Remove player reference from game
	else if (name == "Player")
	{
		Game::GetInstance()->player = nullptr;
	}
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
	Vector2Right.x = 1.0f; Vector2Right.y = 0.0f;
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
		globalPosition = Vector2Add(parent->globalPosition, Vector2Rotate(localPosition, parent->globalRotation));//parent->globalPosition.x + localPosition.x; globalPosition.y = parent->globalPosition.y + localPosition.y;
		globalRotation = parent->globalRotation + localRotation;
	}
	//Clamp rotation
	if (localRotation >= 360) { localRotation -= 360; }
	else if (localRotation < 0) { localRotation += 360; }


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
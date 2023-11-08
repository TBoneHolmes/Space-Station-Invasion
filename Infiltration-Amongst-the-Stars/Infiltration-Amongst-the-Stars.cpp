#include <iostream>
#include "Game.h"

Game game; //Initialize the game

bool isRunning = true; //The game is running

int main()
{
    while (isRunning)
    {
        game.Update();
    }
}
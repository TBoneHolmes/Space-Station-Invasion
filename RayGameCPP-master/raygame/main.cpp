/*******************************************************************************************
*
*   raylib [core] example - basic window
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2013-2016 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include "Game.h"
#include <iostream>
using namespace std;




int main()
{
	// Initialization
	Game* game = Game::GetInstance();
	//--------------------------------------------------------------------------------------
	int screenWidth = game->cameraSize.x;
	int screenHeight = game->cameraSize.y;

	InitWindow(screenWidth, screenHeight, "Space Station Invasion");

	InitAudioDevice();

	SetTargetFPS(60);
	game->Start();
	//--------------------------------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		// Update
		game->Update();

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(BLACK);

		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------   
	UnloadTexture(Game::GetInstance()->spr_title);
	UnloadTexture(Game::GetInstance()->spr_background);
	UnloadTexture(Game::GetInstance()->spr_player);
	UnloadTexture(Game::GetInstance()->spr_playerBooster);
	UnloadTexture(Game::GetInstance()->spr_bullet);
	UnloadTexture(Game::GetInstance()->spr_enemyDefault);
	UnloadTexture(Game::GetInstance()->spr_boss);
	UnloadTexture(Game::GetInstance()->spr_bossShield);
	UnloadTexture(Game::GetInstance()->spr_explosion);
	UnloadTexture(Game::GetInstance()->spr_base);
	UnloadTexture(Game::GetInstance()->spr_asteroidS);
	UnloadTexture(Game::GetInstance()->spr_asteroidM);
	UnloadTexture(Game::GetInstance()->spr_asteroidL);
	UnloadTexture(Game::GetInstance()->spr_powerup);

	UnloadSound(Game::GetInstance()->sfx_boostPlayer);
	UnloadSound(Game::GetInstance()->sfx_shootPlayer);
	UnloadSound(Game::GetInstance()->sfx_shootEnemy);
	UnloadSound(Game::GetInstance()->sfx_hitPlayer);
	UnloadSound(Game::GetInstance()->sfx_hitEnemy);
	UnloadSound(Game::GetInstance()->sfx_hitAsteroid);
	UnloadSound(Game::GetInstance()->sfx_hitBase);
	UnloadSound(Game::GetInstance()->sfx_explodePlayer);
	UnloadSound(Game::GetInstance()->sfx_explodeEnemy);
	UnloadSound(Game::GetInstance()->sfx_explodeAsteroid);
	UnloadSound(Game::GetInstance()->sfx_buttonHover);
	UnloadSound(Game::GetInstance()->sfx_buttonClick);
	UnloadSound(Game::GetInstance()->sfx_pause);
	UnloadSound(Game::GetInstance()->sfx_unpause);
	UnloadSound(Game::GetInstance()->sfx_gameover);
	UnloadSound(Game::GetInstance()->sfx_powerup);
	UnloadSound(Game::GetInstance()->sfx_bossApproach);

	UnloadFont(Game::GetInstance()->fnt_gameover);

	CloseAudioDevice();

	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}
#include "raylib.h"
#include "main.h"
#include "startup.h"

int main(void)
{
	mainGameState = MAIN_GAME_STATE_STARTUP; //tracks what screen the game is displaying
    //set default screen size
    screenWidth = 960;
    screenHeight = 540;

    InitWindow(screenWidth, screenHeight, "Griddy");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);
    
    TraceLog(LOG_INFO, "BeforeLoopStart");
    TraceLog(LOG_INFO, "mainGameState = %d", mainGameState);
    
    while (!WindowShouldClose())
    {
        BeginDrawing();
		//Here is the entire game logic past this while loop game closes
		//First show startup screen
		//Next should be main menu BUT I'm doing a test playground first
		switch (mainGameState)
		{
			case MAIN_GAME_STATE_STARTUP:
				DrawStartup();
			case MAIN_GAME_STATE_TEST_PLAYGROUND:
				DrawTestPlaygroundSetup();
			case MAIN_GAME_STATE_MAIN_MENU:
                break;
		}
        EndDrawing();
    }
    CloseWindow();
    return 0;
}

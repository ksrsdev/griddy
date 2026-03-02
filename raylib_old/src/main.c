#include "raylib.h"
#include "global.h"
#include "main_menu.h"
#include "options_menu.h"
#include "quick_game_confirm.h"
#include "roster_menu.h"
#include "startup.h"
#include "team.h"
#include "team_select.h"
#include "test_playground.h"

#include <stdlib.h>
#include <time.h>

int screenWidth = 960;
int screenHeight = 530;

void CleanupAllMemory(void);

//Global ctx definition
GameContext ctx = {
	.state = MAIN_GAME_STATE_STARTUP,
	.prevState = MAIN_GAME_STATE_NONE,
	.gameRunning = true,

	.errorMsg = {0},
	.isErrorFatal = false,

	.playerTeamId = TEAM_NONE,
	.playerRoster = NULL,
	.playerRosterCount = 0,
	
	.cpuTeamId = TEAM_NONE,
	.cpuRoster = NULL,
	.cpuRosterCount = 0,

	.previewTeamId = TEAM_NONE,
	.previewRoster = NULL,
	.previewRosterCount = 0,
};

int main(void)
{
	//Set random seed - only once per program execution
	srand((unsigned int)time(NULL));
	//raylib
    InitWindow(screenWidth, screenHeight, "Griddy");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);
	//game loop
    while (ctx.gameRunning && !WindowShouldClose())
    {

		//Main Loop is 3 parts post refactor
		//Resize()

		//Resize / scale UI on Window size change
		//Update()

		//Handle all logic (button clicks and sim)

		//Dumb Draw Function
		//Draw()


        BeginDrawing();
		//Here is the entire game logic past this while loop game closes
		//Check for error:
		//Switch on which screen to draw
		switch (ctx.state)
		{
			case MAIN_GAME_STATE_STARTUP:
				DrawStartup();
				break;
			case MAIN_GAME_STATE_TEST_PLAYGROUND:
				DrawTestPlayground();
				break;
			case MAIN_GAME_STATE_MAIN_MENU:
				DrawMainMenu();
                break;
			case MAIN_GAME_STATE_OPTIONS_MENU:
				DrawOptionsMenu();
                break;
			case MAIN_GAME_STATE_QUICK_GAME_PLAYER_TEAM_SELECT:
			case MAIN_GAME_STATE_QUICK_GAME_CPU_TEAM_SELECT:
				DrawQuickGameTeamSelect();
                break;
			case MAIN_GAME_STATE_QUICK_GAME_CONFIRM:
				DrawQuickGameConfirm();
				break;
			case MAIN_GAME_STATE_ROSTER_MENU:
				DrawRosterMenu();
				break;
			case MAIN_GAME_STATE_ERROR:
				DrawErrorScreen();
				break;
			default:
				TraceLog(LOG_ERROR, "ERROR FATAL: ctx.state OOB");
				ctx.gameRunning = false;
				break;
		}
        EndDrawing();
    }
	TraceLog(LOG_INFO, "Shutting Down now :D");
	//Cleanup Memory Leaks here :D
	CleanupAllMemory();
    CloseWindow();
    return 0;
}

void CleanupAllMemory(void)
{
	QuickGameConfirm_UnloadRosters();
	//Roster Menu unload rosters
}

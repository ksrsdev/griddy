#include "raylib.h"
#include "global.h"
#include "main_menu.h"
#include "options_menu.h"
#include "quick_game_confirm.h"
#include "startup.h"
#include "team.h"
#include "team_select.h"
#include "test_playground.h"

#include <stdlib.h>
#include <time.h>

int screenWidth = 960;
int screenHeight = 530;

GameContext griddy = {
	.state = MAIN_GAME_STATE_STARTUP,
	.gameRunning = true,
	.playerTeam = TEAM_NONE,
	.cpuTeam = TEAM_NONE,
};

int main(void)
{
    InitWindow(screenWidth, screenHeight, "Griddy");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);

    TraceLog(LOG_INFO, "BeforeLoopStart");
    TraceLog(LOG_INFO, "mainGameState = %d", griddy.state);

	//Set random seed - only once per program execution
	srand((unsigned int)time(NULL));

    while (griddy.gameRunning && !WindowShouldClose())
    {
        BeginDrawing();
		//Here is the entire game logic past this while loop game closes
		//First show startup screen
		//Next should be main menu BUT I'm doing a test playground first
		switch (griddy.state)
		{
			case MAIN_GAME_STATE_NONE:
			case MAIN_GAME_STATE_COUNT:
				break;
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
		}
        EndDrawing();
    }
    CloseWindow();
    return 0;
}

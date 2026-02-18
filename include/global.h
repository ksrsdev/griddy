#ifndef GUARD_GLOBAL_H
#define GUARD_GLOBAL_H

#include <stdbool.h>

#include "error.h"
#include "player.h"
#include "team.h"


typedef enum {
	MAIN_GAME_STATE_NONE,
	MAIN_GAME_STATE_ERROR,
	MAIN_GAME_STATE_TEST_PLAYGROUND,               
	MAIN_GAME_STATE_STARTUP,    
	MAIN_GAME_STATE_MAIN_MENU,                     
	MAIN_GAME_STATE_OPTIONS_MENU,                     
	MAIN_GAME_STATE_QUICK_GAME_PLAYER_TEAM_SELECT, 
	MAIN_GAME_STATE_QUICK_GAME_CPU_TEAM_SELECT, 
	MAIN_GAME_STATE_QUICK_GAME_CONFIRM,
	MAIN_GAME_STATE_ROSTER_MENU,
	MAIN_GAME_STATE_COUNT
} GameState;

typedef struct {
	//Basic Game Data
	GameState state; //Main Game State
	GameState prevState; //For BACK buttons
	bool gameRunning; //Exit bool
	union {
		MainMenuLayout main_menu;
		OptionsMenuLayout options_menu;
		TeamSelectLayout team_select;
		SummaryScreenLayout summary;
	} layout;
	//Error Data
	char errorMsg[512];
	bool isErrorFatal;
	//Player TeamData (should be a struct)
	TeamId playerTeamId;
	Player *playerRoster;
	long unsigned int playerRosterCount;
	//CPU TeamData (should be a struct)
	TeamId cpuTeamId;
	Player *cpuRoster;
	long unsigned int cpuRosterCount;
	//Preview TeamData (should be a struct) - For Roster Menu
	TeamId previewTeamId;
	Player *previewRoster;
	long unsigned int previewRosterCount;
} GameContext;

//global variables

extern GameContext ctx;

#endif

#ifndef GUARD_GLOBAL_H
#define GUARD_GLOBAL_H

#include <stdbool.h>

#include "player.h"
#include "team.h"


typedef enum {
	MAIN_GAME_STATE_NONE,
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
	GameState state;
	GameState prevState;
	bool gameRunning;
	TeamId playerTeamId;
	TeamId cpuTeamId;
	TeamId previewTeamId;
	Player *playerRoster;
	long unsigned int playerRosterCount;
	Player *cpuRoster;
	long unsigned int cpuRosterCount;
	Player *previewRoster;
	long unsigned int previewRosterCount;
} GameContext;

//global variables

extern GameContext ctx;

#endif

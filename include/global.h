#ifndef GUARD_GLOBAL_H
#define GUARD_GLOBAL_H

#include <stdbool.h>

typedef enum {
	MAIN_GAME_STATE_STARTUP,    
	MAIN_GAME_STATE_MAIN_MENU,                     
	MAIN_GAME_STATE_TEST_PLAYGROUND,               
	MAIN_GAME_STATE_QUICK_GAME_PLAYER_TEAM_SELECT, 
	MAIN_GAME_STATE_QUICK_GAME_CPU_TEAM_SELECT, 
	MAIN_GAME_STATE_QUICK_GAME_CONFIRM
} GameState;

typedef struct {
	GameState state;
	bool gameRunning;
} GameContext;

//global variables

extern GameContext griddy;

#endif

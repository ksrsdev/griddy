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

typedef enum {
	TEAM_SELECTED_NONE,   
	TEAM_SELECTED_RANDOM,
	TEAM_SELECTED_BLACK, 
	TEAM_SELECTED_WHITE,  
	TEAM_SELECTED_GREEN,  
	TEAM_SELECTED_RED,  
	TEAM_SELECTED_PINK,   
	TEAM_SELECTED_BROWN,  
	TEAM_SELECTED_YELLOW, 
	TEAM_SELECTED_ORANGE,
	TEAM_SELECTED_BLUE,
} Team;

typedef struct {
	GameState state;
	bool gameRunning;
	Team playerTeam;
	Team cpuTeam;
} GameContext;

//global variables

extern GameContext griddy;

#endif

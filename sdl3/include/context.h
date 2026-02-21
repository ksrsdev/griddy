#ifndef GUARD_CONTEXT_H
#define GUARD_CONTEXT_H

#include <stdbool.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "intro.h"
#include "main_menu.h"

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
	//SDL Data
	SDL_Window *window;
	SDL_Renderer *renderer;
	TTF_TextEngine *textEngine;
} GameEngine;

typedef struct {
	//Main game info
	bool isRunning;
	GameState state;
	GameState prevState;
	//Error Info
	char errorMsg[512];
	bool isErrorFatal;
	//UI layouts
	union {
		Layout_Intro into;
		Layout_MainMenu mainMenu;
//		Layout_OptionsMenu optionsMenu;
//		Layout_TeamSelect teamSelect;
//		Layout_PreGameSummaryScreen preGameSummary;
	} layout;

} GameData;

typedef struct {
	GameEngine eng;
	GameData data;
} Context;

#endif

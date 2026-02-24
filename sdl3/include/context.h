#ifndef GUARD_CONTEXT_H
#define GUARD_CONTEXT_H

#include <stdbool.h>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

//NOTE: Try not to include anything on this file...causes circular dependency hell
#include "layout.h"

typedef struct {
	int x;
	int y;
} Vector;

typedef struct {
	float x;
	float y;
} FVector;

typedef enum {
	GAME_STATE_NONE,
	GAME_STATE_ERROR,
	GAME_STATE_INTRO,    
	GAME_STATE_MAIN_MENU,                     
//	GAME_STATE_OPTIONS_MENU,                     
//	GAME_STATE_QUICK_GAME_PLAYER_TEAM_SELECT, 
//	GAME_STATE_QUICK_GAME_CPU_TEAM_SELECT, 
//	GAME_STATE_QUICK_GAME_CONFIRM,
//	GAME_STATE_ROSTER_MENU,
	GAME_STATE_COUNT
} GameState;

typedef struct {
	//SDL Data
	SDL_Window *window;
	SDL_Renderer *renderer;
	//TODO: Add resource library
} GameEngine;

typedef struct {
	bool mouseButtonDown;
	bool mouseButtonPressed;
	bool windowResized;
	bool quitRequested;
	Vector newWindowSize;
	Vector mousePos;
	uint8_t keys[SDL_SCANCODE_COUNT];
} GameInput;

typedef struct {
	//Main game info
	bool isRunning;
	GameState newState;
	GameState currState;
	GameState prevState;
	Vector windowSize;

	//Text Engine
	TTF_TextEngine *textEngine;
	TTF_Font *font; //menuFont, buttonFont etc

	//Error Info
	char errorMsg[512];
	bool isErrorFatal;

	//UI layouts
	union {
		IntroLayout    intro;
		MainMenuLayout mainMenu;
//		OptionsMenu optionsMenu;
//		TeamSelect teamSelect;
//		PreGameSummaryScreen preGameSummary;
	} layout;
} GameData;

typedef struct {
	GameEngine eng;
	GameInput input;
	GameData data;
} Context;

typedef void (*TickFunc)(const GameInput *input, GameData *data);
typedef void (*RenderFunc)(const GameEngine *eng, const GameData *data);

#endif

#ifndef GUARD_CONTEXT_H
#define GUARD_CONTEXT_H

#include <stdbool.h>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "types.h"

typedef enum ErrorType;

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
	
	//Text Engine
	TTF_TextEngine *textEngine;
	TTF_Font *font; //menuFont, buttonFont etc

	void *stateResources;
} GameEngine;

typedef struct {
	bool mouseButtonDown;
	bool mouseButtonPressed;
	bool windowResized;
	bool quitRequested;
	Vector2 newWindowSize;
	Vector2 mousePos;
	uint8_t keys[SDL_SCANCODE_COUNT];
} GameInput;

typedef struct {
	//Main game info
	bool isRunning;
	GameState newState;
	GameState currState;
	GameState prevState;
	Vector2 windowSize;

	//Error Info
	char errorMsg[512];
	ErrorType errorType;

	//StateData
	void *stateData;

} GameData;

typedef struct {
	GameEngine eng;
	GameInput input;
	GameData data;
} Context;

#endif

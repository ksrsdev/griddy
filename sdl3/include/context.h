#ifndef GUARD_CONTEXT_H
#define GUARD_CONTEXT_H

#include <stdbool.h>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

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

	//Error Info
	char errorMsg[512];
	bool isErrorFatal;

	//StateData
	void *stateData;

} GameData;

typedef struct {
	GameEngine eng;
	GameInput input;
	GameData data;
} Context;

typedef void (*TickFunc)(const GameInput *input, GameData *data);
typedef void (*RenderFunc)(const GameEngine *eng, const GameData *data);

#endif

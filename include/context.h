#ifndef GUARD_CONTEXT_H
#define GUARD_CONTEXT_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_shadercross/SDL_shadercross.h>

#include "team.h"
#include "types.h"
#include "error_code.h"

typedef enum {
	MAIN_STATE_NONE,
	MAIN_STATE_ERROR,
	MAIN_STATE_INTRO,    
	MAIN_STATE_MAIN_MENU,                     
	MAIN_STATE_OPTIONS_MENU,                     
	MAIN_STATE_TEAM_SELECT, 
	MAIN_STATE_PRE_GAME_CONFIRM,
//	MAIN_STATE_ROSTER_MENU,
	MAIN_STATE_COUNT
} GameState;

typedef struct {
	bool isDown;
	bool wasPressed;
	bool wasReleased;
} ButtonState;

typedef struct {
	FVector2 pos;
	ButtonState left;
	ButtonState right;
	ButtonState middle;
	bool moved;
} MouseState;

typedef struct {
	Vector2 size; 
	u64 timeResized;
	bool resized;
} WindowState;

typedef struct {
	//SDL Data
	SDL_Window *window;
	SDL_Renderer *renderer;
	
	//Text Engine
	TTF_TextEngine *textEngine;
	TTF_Font *font;
	SDL_GPURenderState *sdfRenderState;

	void *stateResources;
} GameEngine;

typedef struct {
	//add when ready
	//ButtonState keys[SDL_SCANCODE_COUNT];
	WindowState window;
	MouseState mouse;
	bool quitRequested;
} GameInput;

typedef struct {
	GameState next;
	GameState curr;
	GameState prev;
} StateContext;

typedef struct {
	//Main Loop bool
	bool isRunning;
	
	//Game State Info
	StateContext state;

	//Pointer / Container for state data
	void *stateData;

	//In Game info
	TeamAssignment teamAssignment;

	//Input
	WindowState window;
	MouseState mouse;
	//keys eventually when ready

	//Error Info - should be refactored into a popup system
	char errorMsg[512];
	ErrorCode errorCode;

	//This is for ui elements - should be textureScale or smth
	u8 padding;

} GameData;

typedef struct {
	GameEngine eng;
	GameInput input;
	GameData data;
} Context;

#endif

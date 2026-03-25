#ifndef GUARD_CONTEXT_H
#define GUARD_CONTEXT_H

#include <stdbool.h>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_shadercross/SDL_shadercross.h>

#include "types.h"
#include "error_code.h"

//typedef enum {
//	FONT_SCALE_NONE,   //  tit, lar,  md,   sm   MAX
//	FONT_SCALE_SMALL,  //  128,  64,  32,  16   1200x720
//	FONT_SCALE_MEDIUM, //  256, 128,  64,  32   1920x1080
//	FONT_SCALE_LARGE,  //  512, 256, 128,  64   3840x2160 
//	FONT_SCALE_MAX,    // 1028, 512, 256, 128   7680x4320
//	FONT_SCALE_COUNT
//} FontScale;


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
	//Main Loop bool
	bool isRunning;
	
	//State Info
	GameState newState;
	GameState currState;
	GameState prevState;
	void *stateData;

	//Input
	WindowState window;
	MouseState mouse;
	//keys eventually when ready

	//Error Info
	char errorMsg[512];
	ErrorCode errorCode;

	u8 padding;

} GameData;

typedef struct {
	GameEngine eng;
	GameInput input;
	GameData data;
} Context;

#endif

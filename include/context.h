#ifndef GUARD_CONTEXT_H
#define GUARD_CONTEXT_H

#include <stdbool.h>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "types.h"
#include "error_code.h"

typedef enum {
	FONT_SCALE_NONE,   //Init, OOB          tit, lar, md, sm
	FONT_SCALE_SMALL,  //720p and below      64,  32, 16,  8
	FONT_SCALE_MEDIUM, //720 to 1080 ish?   128,  64, 32, 16
	FONT_SCALE_LARGE,  //4K?                256, 128, 64, 32   
	FONT_SCALE_COUNT
} FontScale;

typedef struct {
	TTF_Font *title;
	TTF_Font *large;
	TTF_Font *medium;
	TTF_Font *small;
	FontScale scale;
} FontData;

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
	bool resized;
	Vector2 size;
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
	FontData fonts;
	TTF_Font *fontTitle; //Intro Title
	TTF_Font *fontLarge; //Menu Titles?
	TTF_Font *fontMedium; //Buttons, menu info
	TTF_Font *fontSmall; //HUD, jersey numbers, smol

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

} GameData;

typedef struct {
	GameEngine eng;
	GameInput input;
	GameData data;
} Context;

#endif

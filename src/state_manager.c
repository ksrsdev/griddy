#include "state_manager.h"

#include <stdio.h>
#include <stdlib.h>

#include "context.h"
#include "error.h"
#include "intro.h"
#include "main_menu.h"


//   ***   STATIC FUNCTION DECLARATIONS   ***  

static void None_Init(GameEngine *eng, GameData *data);
static void None_Cleanup(GameEngine *eng, GameData *data);

//   ***   LOOKUP TABLES   *** 

static const InitFunc InitTable[] = {
	[GAME_STATE_NONE]      = None_Init,
	[GAME_STATE_ERROR]     = Error_Init,
	[GAME_STATE_INTRO]     = Intro_Init,
	[GAME_STATE_MAIN_MENU] = MainMenu_Init,
};

static const CleanupFunc CleanupTable[] = {
	[GAME_STATE_NONE]      = None_Cleanup,
	[GAME_STATE_ERROR]     = Error_Cleanup,
	[GAME_STATE_INTRO]     = Intro_Cleanup,
	[GAME_STATE_MAIN_MENU] = MainMenu_Cleanup,
};

//   ***   FUNCTION DEFINITIONS   ***  

void StateManager(GameEngine *eng, GameData *data)
{
	//Handle Current Screen cleanup
	CleanupCurrentState(eng, data);

	//Check newState valid
	if (data->newState <= GAME_STATE_NONE || data->newState >= GAME_STATE_COUNT) {
		//ERROR
		data->newState = GAME_STATE_ERROR;
		snprintf(data->errorMsg, sizeof(data->errorMsg), "Invalid State Transition");
	}

	//Assignment
	data->prevState = data->currState;
	data->currState = data->newState;

	//Init New State
	InitFunc initFunc = InitTable[data->currState];
	if (initFunc) {
		initFunc(eng, data);
	}
}

void CleanupCurrentState(GameEngine *eng, GameData *data)
{
	//State specific cleanup (TTF objects, allocated memory etc)
	CleanupFunc cleanupFunc = CleanupTable[data->currState];
	if (cleanupFunc) {
		cleanupFunc(eng, data);
	}

	//Every cleanup will need you to free the memory and null the pointer so just do it every time
	if (data->stateData != NULL) {
		free(data->stateData);
		data->stateData = NULL;
	}

	if (eng->stateResources != NULL) {
		free(eng->stateResources);
		eng->stateResources = NULL;
	}
	
}

//   ***   PLACEHOLDER FUNCS FOR THE LOOKUP TABLES   ***

static void None_Init(GameEngine *eng, GameData *data)
{
	(void)data;
	(void)eng;
	//ERROR
}

static void None_Cleanup(GameEngine *eng, GameData *data)
{
	(void)data;
	(void)eng;
	//ERROR
}

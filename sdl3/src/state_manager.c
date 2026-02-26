#include "state_manager.h"

#include <stdio.h>

#include "context.h"
#include "error.h"
#include "intro.h"
#include "main_menu.h"


//   ***   STATIC FUNCTION DECLARATIONS   ***  

static void NoneInit(GameEngine *eng, GameData *data);
static void NoneCleanup(GameEngine *eng, GameData *data);

//   ***   LOOKUP TABLES   *** 

static const InitFunc InitTable[] = {
	[GAME_STATE_NONE]      = NoneInit,
	[GAME_STATE_ERROR]     = ErrorInit,
	[GAME_STATE_INTRO]     = IntroInit,
	[GAME_STATE_MAIN_MENU] = MainMenuInit,
};

static const CleanupFunc CleanupTable[] = {
	[GAME_STATE_NONE]      = NoneCleanup,
	[GAME_STATE_ERROR]     = ErrorCleanup,
	[GAME_STATE_INTRO]     = IntroCleanup,
	[GAME_STATE_MAIN_MENU] = MainMenuCleanup,
};

//   ***   FUNCTION DEFINITIONS   ***  

void StateManager(GameEngine *eng, GameData *data)
{
	//Handle Current Screen cleanup
	CleanupCurrentState(eng, data);

	//Check newState valid
	if (data->newState <= GAME_STATE_NONE || data->newState >= GAME_STATE_COUNT) {
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

	//Double check to ensure void pointers were freed and NULLd?
	
}

//   ***   PLACEHOLDER FUNCS FOR THE LOOKUP TABLES   ***

static void NoneInit(GameEngine *eng, GameData *data)
{
	(void)data;
	(void)eng;
}

static void NoneCleanup(GameEngine *eng, GameData *data)
{
	(void)data;
	(void)eng;
}

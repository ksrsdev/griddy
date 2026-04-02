#include "state_manager.h"

#include <stdio.h>
#include <stdlib.h>

#include "context.h"
#include "error.h"
#include "intro.h"
#include "main_menu.h"
#include "options_menu.h"


//   ***   STATIC FUNCTION DECLARATIONS   ***  

static void None_Init(GameEngine *eng, GameData *data);
static void None_Cleanup(GameEngine *eng, GameData *data);

//   ***   LOOKUP TABLES   *** 

static const InitFunc InitTable[] = {
	[GAME_STATE_NONE]         = None_Init,
	[GAME_STATE_ERROR]        = Error_Init,
	[GAME_STATE_INTRO]        = Intro_Init,
	[GAME_STATE_MAIN_MENU]    = MainMenu_Init,
	[GAME_STATE_OPTIONS_MENU] = OptionsMenu_Init,
	[GAME_STATE_TEAM_SELECT]  = TeamSelect_Init,
};

static const CleanupFunc CleanupTable[] = {
	[GAME_STATE_NONE]         = None_Cleanup,
	[GAME_STATE_ERROR]        = Error_Cleanup,
	[GAME_STATE_INTRO]        = Intro_Cleanup,
	[GAME_STATE_MAIN_MENU]    = MainMenu_Cleanup,
	[GAME_STATE_OPTIONS_MENU] = OptionsMenu_Cleanup,
	[GAME_STATE_TEAM_SELECT]  = TeamSelect_Cleanup,
};

//   ***   FUNCTION DEFINITIONS   ***  

void StateManager(GameEngine *eng, GameData *data)
{
	//Handle Current Screen cleanup
	CleanupCurrentState(eng, data);

	//Check nextState valid
	if (data->state.next <= GAME_STATE_NONE || data->state.next >= GAME_STATE_COUNT) {
		//ERROR
		data->state.next = GAME_STATE_ERROR;
		snprintf(data->errorMsg, sizeof(data->errorMsg), "Invalid State Transition");
	}

	//Assignment
	data->state.prev = data->state.curr;
	data->state.curr = data->state.next;

	//Init New State
	InitFunc initFunc = InitTable[data->state.curr];
	if (initFunc) {
		initFunc(eng, data);
	}
}

void CleanupCurrentState(GameEngine *eng, GameData *data)
{
	//State specific cleanup (TTF objects, allocated memory etc)
	CleanupFunc cleanupFunc = CleanupTable[data->state.curr];
	if (cleanupFunc) {
		cleanupFunc(eng, data);
	}

	//Every cleanup will need you to free the memory and null the pointer so just do it every time
	if (data->stateData != nullptr) {
		free(data->stateData);
		data->stateData = nullptr;
	}

	if (eng->stateResources != nullptr) {
		free(eng->stateResources);
		eng->stateResources = nullptr;
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

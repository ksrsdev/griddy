#include "core.h"

#include <stdio.h>

#include "context.h"
#include "error.h"
#include "intro.h"
#include "main_menu.h"

//   ***   STATIC FUNCTION DECLARATIONS   *** 

//core helper funcs
static void UpdateGameState(GameData *data, GameState newState); 
static void UpdateWindowSize(const Vector newWindowSize, Vector *windowSize);
static void UpdateGameState(GameData *data, GameState newState);
static void CleanupCurrentState(GameData *data);

//Core Tick Funcs
static void NoneInit(GameData *data);
static void NoneTick(const GameInput *input, GameData *data);
static void NoneCleanup(GameData *data);

//   ***   LOOKUP TABLES   *** 

static const InitFunc InitTable[] = {
	[GAME_STATE_NONE]      = NoneInit,
	[GAME_STATE_ERROR]     = ErrorInit,
	[GAME_STATE_INTRO]     = IntroInit,
	[GAME_STATE_MAIN_MENU] = MainMenuInit,
};

static const TickFunc TickTable[] = {
	[GAME_STATE_NONE]      = NoneTick,
	[GAME_STATE_ERROR]     = ErrorTick,
	[GAME_STATE_INTRO]     = IntroTick,
	[GAME_STATE_MAIN_MENU] = MainMenuTick,
};

static const CleanupFunc CleanupTable[] = {
	[GAME_STATE_NONE]      = NoneCleanup,
	[GAME_STATE_ERROR]     = ErrorCleanup,
	[GAME_STATE_INTRO]     = IntroCleanup,
	[GAME_STATE_MAIN_MENU] = MainMenuCleanup,
};

//   ***   FUNCTION DEFINITIONS   *** 

void CoreTick(const GameInput *input, GameData *data)
{
	//Handle Quit Request
	if (input->quitRequested) {
		//Cleanup current state data (TTF_Text objects etc)
		CleanupCurrentState(data);

		//Quit Main Loop
		data->isRunning = false;
		return;
	}
	
	//Handle Resize
	if (input->windowResized) {
		UpdateWindowSize(input->newWindowSize, &data->windowSize);
	}

	//Run correct TickFunc for current GameState
	if (data->state >= GAME_STATE_NONE && data->state < GAME_STATE_COUNT) {
		TickFunc tickFunc = TickTable[data->state];
        if (tickFunc) {
            tickFunc(input, data);
        }
	} else {
		printf("ERROR: GameState: %d OOB in Core_Trick()\n", data->state);
	}
}

static void NoneTick(const GameInput *input, GameData *data)
{
	(void)input;

	//Transition to Intro Screen
	UpdateGameState(data, GAME_STATE_INTRO);
}

//Nothing to init it's just a placeholder for the func table
static void NoneInit(GameData *data)
{
	(void)data;
}

//Nothing to cleanup it's just a placeholder for the func table
static void NoneCleanup(GameData *data)
{
	(void)data;
}

static void UpdateWindowSize(const Vector newWindowSize, Vector *windowSize)
{
	windowSize->x = newWindowSize.x;
	windowSize->y = newWindowSize.y;
}

static void UpdateGameState(GameData *data, GameState newState) {

	//Handle Current Screen cleanup
	CleanupCurrentState(data);

	//Check newState valid
	if (newState <= GAME_STATE_NONE || newState >= GAME_STATE_COUNT) {
		newState = GAME_STATE_ERROR;
		snprintf(data->errorMsg, sizeof(data->errorMsg), "Invalid State Transition");
	}

	//Assignment
	data->prevState = data->state;
	data->state = newState;

	//Init New State
	InitFunc initFunc = InitTable[data->state];
	if (initFunc) {
		initFunc(data);
	}
}

static void CleanupCurrentState(GameData *data)
{
	//State specific cleanup (TTF objects, allocated memory etc)
	CleanupFunc cleanupFunc = CleanupTable[data->state];
	if (cleanupFunc) {
		cleanupFunc(data);
	}
	
	//zero layou data every time you clean up
	memset (&data->layout, 0, sizeof(data->layout));
}

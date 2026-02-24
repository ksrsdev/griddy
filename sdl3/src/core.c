#include "core.h"

#include <stdio.h>

#include "context.h"
#include "error.h"
#include "intro.h"
#include "main_menu.h"

//   ***   STATIC FUNCTION DECLARATIONS   *** 

//core helper funcs
static void UpdateWindowSize(const Vector newWindowSize, Vector *windowSize);

static void NoneTick(const GameInput *input, GameData *data);

//   ***   LOOKUP TABLES   *** 

static const TickFunc TickTable[] = {
	[GAME_STATE_NONE]      = NoneTick,
	[GAME_STATE_ERROR]     = ErrorTick,
	[GAME_STATE_INTRO]     = IntroTick,
	[GAME_STATE_MAIN_MENU] = MainMenuTick,
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

static void UpdateWindowSize(const Vector newWindowSize, Vector *windowSize)
{
	windowSize->x = newWindowSize.x;
	windowSize->y = newWindowSize.y;
}

//TODO: This should mostly be handled by StateManager. This function (rename pls) Should only set newState
void RequestGameStateTransition(GameData *data, const GameState newState)
{
	data->newState = newState;
}

static void NoneTick(const GameInput *input, GameData *data)
{
	(void)input;

	//Transition to Intro Screen
	UpdateGameState(data, GAME_STATE_INTRO);
}

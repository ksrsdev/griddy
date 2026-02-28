#include "update.h"

#include <stdio.h>

#include "context.h"
#include "error.h"
#include "intro.h"
#include "main_menu.h"

//   ***   STATIC FUNCTION DECLARATIONS   *** 

//core helper funcs
static void UpdateWindowSize(const Vector newWindowSize, Vector *windowSize);

static void None_Update(const GameInput *input, GameData *data);

//   ***   LOOKUP TABLES   *** 

static const UpdateFunc UpdateTable[] = {
	[GAME_STATE_NONE]      = None_Update,
	[GAME_STATE_ERROR]     = Error_Update,
	[GAME_STATE_INTRO]     = Intro_Update,
	[GAME_STATE_MAIN_MENU] = MainMenu_Update,
};

//   ***   FUNCTION DEFINITIONS   *** 

void Main_Update(const GameInput *input, GameData *data)
{

	//TODO: UpdateGameData (Copy data from input to data
	//THEN: Run correct UpdateFunc 

	//Handle Quit Request
	if (input->quitRequested) {
		//Quit Main Loop
		data->isRunning = false;
		return;
	}
	
	//Handle Resize
	if (input->windowResized) {
		UpdateWindowSize(input->newWindowSize, &data->windowSize);
	}

	//Run correct UpdateFunc for current GameState
	if (data->currState >= GAME_STATE_NONE && data->currState < GAME_STATE_COUNT) {
		UpdateFunc updateFunc = UpdateTable[data->currState];
        if (updateFunc) {
            updateFunc(input, data);
        }
	} else {
		printf("ERROR: GameState: %d OOB in Core_Trick()\n", data->currState);
	}
}

static void UpdateWindowSize(const Vector newWindowSize, Vector *windowSize)
{
	windowSize->x = newWindowSize.x;
	windowSize->y = newWindowSize.y;
}

void RequestGameStateTransition(GameData *data, const GameState newState)
{
	data->newState = newState;
}

static void None_Update(const GameInput *input, GameData *data)
{
	(void)input;

	//Transition to Intro Screen
	RequestGameStateTransition(data, GAME_STATE_INTRO);
}

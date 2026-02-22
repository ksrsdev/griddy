#include <stdio.h>

#include "context.h"
#include "core.h"
#include "intro.h"

//Should be in respective .c file methinks
void Tick_Error(const GameInput *input, GameData *data);
void Tick_MainMenu(const GameInput *input, GameData *data);

//Actual relevant functions that belong here
static void Tick_None(const GameInput *input, GameData *data);
static void UpdateGameState(GameState *state, GameState *prevState, GameState newState); 

static const TickFunc TickTable[] = {
	[GAME_STATE_NONE]      = Tick_None,
	[GAME_STATE_ERROR]     = Tick_Error,
	[GAME_STATE_INTRO]     = Tick_Intro,
	[GAME_STATE_MAIN_MENU] = Tick_MainMenu,
};

//Should be in respective .c file methinks
void Tick_Error(const GameInput *input, GameData *data)
{
	(void)input;
	(void)data;
}

void Tick_MainMenu(const GameInput *input, GameData *data)
{
	(void)input;
	(void)data;
}

//Actual functions that belong here (I think!)
static void Tick_None(const GameInput *input, GameData *data)
{
	(void)input;
	//Setup Intro Screen
	printf("Here to update Game State");
	UpdateGameState(&data->state, &data->prevState, GAME_STATE_INTRO);
}

static void UpdateWindowSize(const Vector newWindowSize, Vector *windowSize)
{
	printf("UpdateWindowSize() - nX: %d\nnY: %d\n", newWindowSize.x, newWindowSize.y);
	windowSize->x = newWindowSize.x;
	windowSize->y = newWindowSize.y;
}

static void UpdateGameState(GameState *state, GameState *prevState, GameState newState) {
	if (newState <= GAME_STATE_NONE || newState >= GAME_STATE_COUNT) {
		*state = GAME_STATE_ERROR;
		return;
	}
	*prevState = *state;
	*state = newState;
}

//Master Logic Brain
void Core_Tick(const GameInput *input, GameData *data)
{

	//Handle engine inputs (Resize, Quit)
	
	//Resize
	if (input->windowResized) {
		UpdateWindowSize(input->newWindowSize, &data->windowSize);
	}

	//Handle Quit request
	if (input->quitRequested) {
		data->isRunning = false;
	}
	
	if (data->state >= GAME_STATE_NONE && data->state < GAME_STATE_COUNT) {
		TickFunc tickFunc = TickTable[data->state];
        if (tickFunc) {
            tickFunc(input, data);
        }
	} else {
		printf("ERROR: GameState: %d OOB in Core_Trick()\n", data->state);
	}
}

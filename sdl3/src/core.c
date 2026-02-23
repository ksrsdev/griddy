#include <stdio.h>

#include "context.h"
#include "core.h"
#include "intro.h"

//Should be in respective .c file methinks
void Tick_Error(const GameInput *input, GameData *data);
void Tick_MainMenu(const GameInput *input, GameData *data);

//Actual relevant functions that belong here
static void Tick_None(const GameInput *input, GameData *data);
static void UpdateGameState(GameData *data, GameState newState); 

//FIXME: State_Tick not Tick_State
static const TickFunc TickTable[] = {
	[GAME_STATE_NONE]      = Tick_None,
	[GAME_STATE_ERROR]     = Tick_Error,
	[GAME_STATE_INTRO]     = Intro_Tick,
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
	UpdateGameState(data, GAME_STATE_INTRO);
	Intro_Init(data);
	data->layout.intro.startTime = SDL_GetTicks();
}

static void UpdateWindowSize(const Vector newWindowSize, Vector *windowSize)
{
	windowSize->x = newWindowSize.x;
	windowSize->y = newWindowSize.y;
}

static void UpdateGameState(GameData *data, GameState newState) {
	if (newState <= GAME_STATE_NONE || newState >= GAME_STATE_COUNT) {
		newState = GAME_STATE_ERROR;
		snprintf(data->errorMsg, sizeof(data->errorMsg), "Invalid State Transition");
	}

	data->prevState = data->state;
	data->state = newState;
	memset (&data->layout, 0, sizeof(data->layout));
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

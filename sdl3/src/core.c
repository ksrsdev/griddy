#include <stdio.h>

#include "context.h"
#include "core.h"
#include "intro.h"

//Should be in respective .c file methinks
void Tick_Error(const GameInput *input, GameData *data);
void Error_Cleanup(GameData *data);
void Tick_MainMenu(const GameInput *input, GameData *data);
void MainMenu_Cleanup(GameData *data);

//Actual relevant functions that belong here
static void None_Tick(const GameInput *input, GameData *data);
static void None_Cleanup(GameData *data);
static void UpdateGameState(GameData *data, GameState newState); 
static void UpdateWindowSize(const Vector newWindowSize, Vector *windowSize);
static void UpdateGameState(GameData *data, GameState newState);
static void CleanupCurrentState(GameData *data);

//FIXME: State_Tick not Tick_State
static const TickFunc TickTable[] = {
	[GAME_STATE_NONE]      = None_Tick,
	[GAME_STATE_ERROR]     = Tick_Error,
	[GAME_STATE_INTRO]     = Intro_Tick,
	[GAME_STATE_MAIN_MENU] = Tick_MainMenu,
};

static const CleanupFunc CleanupTable[] = {
	[GAME_STATE_NONE]      = None_Cleanup,
	[GAME_STATE_ERROR]     = Error_Cleanup,
	[GAME_STATE_INTRO]     = Intro_Cleanup,
	[GAME_STATE_MAIN_MENU] = MainMenu_Cleanup,
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

void Error_Cleanup(GameData *data)
{
	(void)data;
}

void MainMenu_Cleanup(GameData *data)
{
	(void)data;
}

//Actual functions that belong here (I think!)
//Master Logic Brain
void Core_Tick(const GameInput *input, GameData *data)
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

static void None_Tick(const GameInput *input, GameData *data)
{
	(void)input;

	//Setup Intro Screen
	UpdateGameState(data, GAME_STATE_INTRO);
	Intro_Init(data);
	data->layout.intro.startTime = SDL_GetTicks();
}

//Nothing to cleanup it's just a placeholder for the func table
static void None_Cleanup(GameData *data)
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

	//Zero layout union data
	memset (&data->layout, 0, sizeof(data->layout));

	//TODO
	//Init Table Func
}

static void CleanupCurrentState(GameData *data)
{
	CleanupFunc cleanupFunc = CleanupTable[data->state];
	if (cleanupFunc)   {
		cleanupFunc(data);
	}
}

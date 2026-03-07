#include "update.h"

#include "context.h"
#include "error.h"
#include "intro.h"
#include "main_menu.h"

//   ***   STATIC FUNCTION DECLARATIONS   *** 

//core helper funcs
static void UpdateGameData(const GameInput *input, GameData *data);
static void UpdateWindowSize(const Vector2 newWindowSize, Vector2 *windowSize);
static void ResetGameDataInputBools(WindowState *window, MouseState *mouse);

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
	UpdateGameData(input, data);
	
	//Quit ASAP if quitRequested
	if (data->isRunning == false) {
		return;
	}

	//Run correct UpdateFunc for current GameState
	if (data->currState >= GAME_STATE_NONE && data->currState < GAME_STATE_COUNT) {
		UpdateFunc updateFunc = UpdateTable[data->currState];
        if (updateFunc) {
            updateFunc(input, data);
        }
	} else {
		//ERROR
		//printf("ERROR: GameState: %d OOB in Core_Trick()\n", data->currState);s
		return;
	}
}

static void UpdateGameData(const GameInput *input, GameData *data)
{
	//Handle Quit Request ASAP
	if (input->quitRequested) {
		data->isRunning = false;
		return;
	}

	ResetGameDataInputBools(&data->window, &data->mouse);	
	
	//Handle Resize
	if (input->window.resized) {
		UpdateWindowSize(input->window.size, &data->window.size);
		data->window.resized = true;
	}

	//Handle Mouse

	//TODO
//	if (input->mousePos

	

}

//Default false. Set true during check inputs phase if needed
static void ResetGameDataInputBools(WindowState *window, MouseState *mouse)
{
	window->resized = false;

	mouse->moved = false;
	mouse->left.wasPressed = false;
	mouse->left.wasReleased = false;
	mouse->right.wasPressed = false;
	mouse->right.wasReleased = false;
	mouse->middle.wasPressed = false;
	mouse->middle.wasReleased = false;
}

static void UpdateWindowSize(const Vector2 newWindowSize, Vector2 *windowSize)
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

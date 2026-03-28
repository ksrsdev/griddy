#include "update.h"

#include "context.h"
#include "error.h"
#include "intro.h"
#include "main_menu.h"

//   ***   STATIC FUNCTION DECLARATIONS   *** 

//core helper funcs

static void None_Update(GameData *data);

//   ***   LOOKUP TABLES   *** 

static const UpdateFunc UpdateTable[] = {
	[GAME_STATE_NONE]         = None_Update,
	[GAME_STATE_ERROR]        = Error_Update,
	[GAME_STATE_INTRO]        = Intro_Update,
	[GAME_STATE_MAIN_MENU]    = MainMenu_Update,
	[GAME_STATE_OPTIONS_MENU] = OptionsMenu_Update,
	[GAME_STATE_TEAM_SELECT]  = TeamSelect_Update,
};

//   ***   FUNCTION DEFINITIONS   *** 


void Main_Update(GameData *data)
{
	//Run correct UpdateFunc for current GameState
	if (data->currState >= GAME_STATE_NONE && data->currState < GAME_STATE_COUNT) {
		UpdateFunc updateFunc = UpdateTable[data->currState];
        if (updateFunc) {
            updateFunc(data);
        }
	} else {
		//ERROR
		//printf("ERROR: GameState: %d OOB in Core_Trick()\n", data->currState);s
		return;
	}
}

void RequestGameStateTransition(GameData *data, const GameState newState)
{
	data->newState = newState;
}

static void None_Update(GameData *data)
{
	RequestGameStateTransition(data, GAME_STATE_INTRO);
}

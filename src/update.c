#include "update.h"

#include "context.h"
#include "error.h"
#include "intro.h"
#include "main_menu.h"
#include "match.h"
#include "pre_game_confirm.h"

//   ***   STATIC FUNCTION DECLARATIONS   *** 

//core helper funcs

static void None_Update(GameData *data);

//   ***   LOOKUP TABLES   *** 

static const UpdateFunc UpdateTable[] = {
	[MAIN_STATE_NONE]             = None_Update,
	[MAIN_STATE_ERROR]            = Error_Update,
	[MAIN_STATE_INTRO]            = Intro_Update,
	[MAIN_STATE_MAIN_MENU]        = MainMenu_Update,
	[MAIN_STATE_OPTIONS_MENU]     = OptionsMenu_Update,
	[MAIN_STATE_TEAM_SELECT]      = TeamSelect_Update,
	[MAIN_STATE_PRE_GAME_CONFIRM] = PreGameConfirm_Update,
	[MAIN_STATE_MATCH]            = Match_Update,
};

//   ***   FUNCTION DEFINITIONS   *** 


void Main_Update(GameData *data)
{
	//Run correct UpdateFunc for current GameState
	if (data->state.curr >= MAIN_STATE_NONE && data->state.curr < MAIN_STATE_COUNT) {
		UpdateFunc updateFunc = UpdateTable[data->state.curr];
        if (updateFunc) {
            updateFunc(data);
        }
	} else {
		//ERROR
		//printf("ERROR: GameState: %d OOB in Core_Trick()\n", data->state.curr);
		return;
	}
}

void RequestGameStateTransition(GameData *data, const GameState nextState)
{
	data->state.next = nextState;
}

static void None_Update(GameData *data)
{
	RequestGameStateTransition(data, MAIN_STATE_INTRO);
}

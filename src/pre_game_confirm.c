#include "pre_game_confirm.h"

#include <stdlib.h>

#include "colors.h"
#include "init.h"
#include "state_data.h"

static void PreGameConfirm_LoadUIStrings(const GameData *data);
static void PreGameConfirm_InitUIData(const GameEngine *eng, const GameData *data);

//   ###   INIT   ###
void PreGameConfirm_Init(GameEngine *eng, GameData *data)
{
	data->stateData = calloc(1, sizeof(PreGameConfirmData));
	if (data->stateData == nullptr) {
		//error.c errors are fatal
		Error_Alert(data, ERROR_ALLOC, "opionsMenuData failed calloc()");
		return;
	}

	//TODO: Resolve Random Team

	PreGameConfirm_LoadUIStrings(data);
	PreGameConfirm_InitUIData(eng, data);

}

void PreGameConfirm_Cleanup(GameEngine *eng, GameData *data)
{
	(void)eng;

	PreGameConfirmData *preGameConfirmData = data->stateData;

	for (s32 i = PRE_GAME_CONFIRM_UI_START; i < PRE_GAME_CONFIRM_UI_END; i++) {

		UIData *uiData = &preGameConfirmData->uiData[i];
		if (uiData->texture) {
			SDL_DestroyTexture(uiData->texture);
			uiData->texture = nullptr;
		}
	}

	Deinit_StateData(&data->stateData);
}

//   ###   UPDATE   ###
void PreGameConfirm_Update(GameData *data)
{
	(void)data;

}

//   ###   UPDATE   ###
void PreGameConfirm_Render(const GameEngine *eng, const GameData *data)
{
	(void)eng;
	(void)data;

}

static void PreGameConfirm_LoadUIStrings(const GameData *data)
{
	PreGameConfirmData *preGameConfirm = data->stateData;

	preGameConfirm->uiStrings[PRE_GAME_CONFIRM_UI_TITLE] = "CONFIRM";
	
	preGameConfirm->uiStrings[PRE_GAME_CONFIRM_UI_PLAYER_TITLE] = "PLAYER";
	preGameConfirm->uiStrings[PRE_GAME_CONFIRM_UI_CPU_TITLE] = "CPU";
	
	preGameConfirm->uiStrings[PRE_GAME_CONFIRM_UI_VS] = "VS.";

	TeamID playerTeamID = data->teamAssignment.player;
	TeamID cpuTeamID = data->teamAssignment.cpu;

	TeamDescription playerTeamDesc = gTeamDescriptions[playerTeamID];
	TeamDescription cpuTeamDesc = gTeamDescriptions[cpuTeamID];
	
	preGameConfirm->uiStrings[PRE_GAME_CONFIRM_UI_PLAYER_BOX] = playerTeamDesc.title;
	preGameConfirm->uiStrings[PRE_GAME_CONFIRM_UI_CPU_BOX] = cpuTeamDesc.title;
	
	preGameConfirm->uiStrings[PRE_GAME_CONFIRM_UI_PLAYER_PREVIEW] = "PREVIEW";
	preGameConfirm->uiStrings[PRE_GAME_CONFIRM_UI_CPU_PREVIEW] = "PREVIEW";
	
	preGameConfirm->uiStrings[PRE_GAME_CONFIRM_UI_BACK] = "<-BACK";
	preGameConfirm->uiStrings[PRE_GAME_CONFIRM_UI_PLAY] = "PLAY GAME";
}

static void PreGameConfirm_InitUIData(const GameEngine *eng, const GameData *data)
{
	PreGameConfirmData *preGameConfirmData = data->stateData;
	UIData *uiData = nullptr;

	//TODO sub functions:
	//Setup UI
	//Resize Layout
	//Check initial highlight
	//create textures
	

	//Title
	uiData = &preGameConfirmData->uiData[PRE_GAME_CONFIRM_UI_TITLE];

	uiData->type = UI_TYPE_TEXT;
	uiData->fg = COLOR_BLACK;
	
	//Player Title
	//CPU Title
	//VS
	//Player Box
	//CPU Box
	//Player Preview
	//CPU Preview
	//Back
	//Play
	
	(void)eng;
}

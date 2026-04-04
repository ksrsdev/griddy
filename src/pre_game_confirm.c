#include "pre_game_confirm.h"

#include <stdlib.h>

#include "init.h"
#include "state_data.h"

static void PreGameConfirm_LoadUIStrings(const GameData *data);
//static void PreGameConfirm_LoadUIData(const GameEngine *eng, const GameData *data);

//   ###   INIT   ###
void PreGameConfirm_Init(GameEngine *eng, GameData *data)
{
	data->stateData = calloc(1, sizeof(PreGameConfirmData));
	if (data->stateData == nullptr) {
		//error.c errors are fatal
		Error_Alert(data, ERROR_ALLOC, "opionsMenuData failed calloc()");
		return;
	}

	PreGameConfirm_LoadUIStrings(data);
	(void)eng;

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
	
	preGameConfirm->uiStrings[PRE_GAME_CONFIRM_UI_PLAYER_BOX] = "WIP";
	preGameConfirm->uiStrings[PRE_GAME_CONFIRM_UI_CPU_BOX] = "WIP";
	
	preGameConfirm->uiStrings[PRE_GAME_CONFIRM_UI_PLAYER_PREVIEW] = "PREVIEW";
	preGameConfirm->uiStrings[PRE_GAME_CONFIRM_UI_CPU_PREVIEW] = "PREVIEW";
	
	preGameConfirm->uiStrings[PRE_GAME_CONFIRM_UI_BACK] = "<-BACK";
	preGameConfirm->uiStrings[PRE_GAME_CONFIRM_UI_PLAY] = "PLAY GAME";


}

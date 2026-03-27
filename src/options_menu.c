#include "options_menu.h"

#include <stdlib.h>

#include "context.h"
#include "init.h"
#include "state_data.h"

//   ###   INIT   ###
void OptionsMenu_Init(GameEngine *eng, GameData *data) 
{
	(void)eng;
	data->stateData = calloc(1, sizeof(OptionsMenuData));
	if (data->stateData == NULL) {
		//error.c errors are fatal
		Error_Alert(data, ERROR_ALLOC, "opionsMenuData failed calloc()");
		return;
	}
	
	//remove (void) above when you actually add this stuff

}

//   ###   CLEANUP   ###
void OptionsMenu_Cleanup(GameEngine *eng, GameData *data) 
{
	(void)eng;

	OptionsMenuData *optionsMenuData = data->stateData;

	for (s32 i = OPTIONS_MENU_UI_START; i < OPTIONS_MENU_UI_END; i++) {

		UIData *uiData = &optionsMenuData->uiData[i];
		if (uiData->texture) {
			SDL_DestroyTexture(uiData->texture);
			uiData->texture = NULL;
		}
	}

	Deinit_StateData(&data->stateData);
}

//   ###   UPDATE   ###
void OptionsMenu_Update(GameData *data) 
{
	(void)data;

	//remove (void) above when you actually add this stuff
}

//   ###   RENDER   ###
void OptionsMenu_Render(const GameEngine *eng, const GameData *data) 
{
	(void)eng;
	(void)data;

	//remove (void) above when you actually add this stuff

}

//   ###   HELPERS   ###

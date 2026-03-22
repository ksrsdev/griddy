#include "main_menu.h"

#include <stdlib.h>

#include "context.h"
#include "error.h"
#include "init.h"
#include "state_data.h"
#include "ui.h"

static void MainMenu_LoadUIStrings(const GameData *data);

//   ***   FUNCTION DEFINITIONS   ***

//INIT & DEINIT
void MainMenu_Init(GameEngine *eng, GameData *data)
{
	(void)eng;
	data->stateData = calloc(1, sizeof(MainMenuData));
	if (data->stateData == NULL) {
		//error.c errors are fatal
		Error_Alert(data, ERROR_ALLOC, "mainMenuData failed calloc()");
		return;
	}

	MainMenu_LoadUIStrings(data);

	//UNVOID eng above when you do this stuff
//	MainMenu_LoadUIData(eng, data);
}

void MainMenu_Cleanup(GameEngine *eng, GameData *data)
{
	(void)eng;

	Deinit_StateData(&data->stateData);

}

//UPDATE

void MainMenu_Update(GameData *data)
{
	(void)data;
}

//RENDER

void MainMenu_Render(const GameEngine *eng, const GameData *data)
{
	(void)eng;
	(void)data;
}


static void MainMenu_LoadUIStrings(const GameData *data)
{

	MainMenuData *mainMenuData = data->stateData;

	mainMenuData->uiStrings[MAIN_MENU_UI_TITLE]   = "GRIDDY";
	mainMenuData->uiStrings[MAIN_MENU_UI_SPLASH]  = "SHAKE IT!";
	mainMenuData->uiStrings[MAIN_MENU_UI_PLAY]    = "PLAY GAME";
	mainMenuData->uiStrings[MAIN_MENU_UI_OPTIONS] = "OPTIONS";
	mainMenuData->uiStrings[MAIN_MENU_UI_EXIT]    = "EXIT";
	mainMenuData->uiStrings[MAIN_MENU_UI_VERSION] = "SDL_Test";

}



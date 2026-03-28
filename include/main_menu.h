#ifndef GUARD_MAIN_MENU_H
#define GUARD_MAIN_MENU_H

#include "context.h"

typedef enum {
	MAIN_MENU_UI_NONE,

	//Text
	MAIN_MENU_UI_TITLE,
	MAIN_MENU_UI_SPLASH,
	MAIN_MENU_UI_VERSION,

	//Buttons
	MAIN_MENU_UI_PLAY,
	MAIN_MENU_UI_OPTIONS,
	MAIN_MENU_UI_EXIT,

	//Count
	MAIN_MENU_UI_COUNT,

	//Aliases
	MAIN_MENU_UI_START = MAIN_MENU_UI_NONE + 1,
	MAIN_MENU_UI_END   = MAIN_MENU_UI_COUNT,

	MAIN_MENU_UI_BUTTON_START = MAIN_MENU_UI_PLAY,
	MAIN_MENU_UI_BUTTON_END   = MAIN_MENU_UI_EXIT + 1
} MainMenuUIElement;

void MainMenu_Init(GameEngine *eng, GameData *data);
void MainMenu_Cleanup(GameEngine *eng, GameData *data);
void MainMenu_Update(GameData *data);
void MainMenu_Render(const GameEngine *eng, const GameData *data);

#endif

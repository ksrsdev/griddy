#ifndef GUARD_MAIN_MENU_H
#define GUARD_MAIN_MENU_H

#include "context.h"

typedef enum {
	MAIN_MENU_UI_NONE,
	MAIN_MENU_UI_TITLE,
	MAIN_MENU_UI_SPLASH,
	MAIN_MENU_UI_PLAY,
	MAIN_MENU_UI_OPTIONS,
	MAIN_MENU_UI_EXIT,
	MAIN_MENU_UI_VERSION,
	MAIN_MENU_UI_COUNT
} MainMenuUIElements;

void MainMenu_Init(GameEngine *eng, GameData *data);
void MainMenu_Update(GameData *data);
void MainMenu_Render(const GameEngine *eng, const GameData *data);
void MainMenu_Cleanup(GameEngine *eng, GameData *data);

#endif

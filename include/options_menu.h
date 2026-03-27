#ifndef GUARD_PPTIONS_MENU_H
#define GUARD_PPTIONS_MENU_H

#include "context.h"

typedef enum {
	OPTIONS_MENU_UI_NONE,
	OPTIONS_MENU_UI_TITLE,
	OPTIONS_MENU_UI_GEN_NEW_ROSTERS,
	OPTIONS_MENU_UI_BACK,
	OPTIONS_MENU_UI_COUNT,

	//Aliases
	OPTIONS_MENU_UI_START = OPTIONS_MENU_UI_NONE + 1,
	OPTIONS_MENU_UI_END   = OPTIONS_MENU_UI_COUNT,

} OptionsMenuUIElement;

void OptionsMenu_Init(GameEngine *eng, GameData *data);
void OptionsMenu_Cleanup(GameEngine *eng, GameData *data);
void OptionsMenu_Update(GameData *data);
void OptionsMenu_Render(const GameEngine *eng, const GameData *data);

#endif

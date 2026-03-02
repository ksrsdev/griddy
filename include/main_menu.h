#ifndef GUARD_MAIN_MENU_H
#define GUARD_MAIN_MENU_H

#include "context.h"


void MainMenu_Init(GameEngine *eng, GameData *data);
void MainMenu_Update(const GameInput *input, GameData *data);
void MainMenu_Render(const GameEngine *eng, const GameData *data);
void MainMenu_Cleanup(GameEngine *eng, GameData *data);

#endif

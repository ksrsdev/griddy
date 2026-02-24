#ifndef GUARD_MAIN_MENU_H
#define GUARD_MAIN_MENU_H

#include "context.h"


void MainMenuInit(GameEngine *eng, GameData *data);
void MainMenuTick(const GameInput *input, GameData *data);
void MainMenuRender(const GameEngine *eng, const GameData *data);
void MainMenuCleanup(GameEngine *eng, GameData *data);

#endif

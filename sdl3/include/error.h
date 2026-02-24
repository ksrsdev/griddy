#ifndef GUARD_ERROR_H
#define GUARD_ERROR_H

#include "context.h"

void ErrorInit(GameData *data);
void ErrorTick(const GameInput *input, GameData *data);
void ErrorRender(const GameEngine *eng, const GameData *data);
void ErrorCleanup(GameData *data);

#endif

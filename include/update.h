#ifndef GUARD_CORE_H
#define GUARD_CORE_H

#include "context.h"

typedef void (*UpdateFunc)(const GameInput *input, GameData *data);

void Main_Update(const GameInput *input, GameData *data);
void RequestGameStateTransition(GameData *data, const GameState newState);

#endif

#ifndef GUARD_CORE_H
#define GUARD_CORE_H

#include "context.h"


void CoreTick(const GameInput *input, GameData *data);
void RequestGameStateTransition(GameData *data, const GameState newState);


#endif

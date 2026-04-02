#ifndef GUARD_CORE_H
#define GUARD_CORE_H

#include "context.h"

typedef void (*UpdateFunc)(GameData *data);

void Update_SyncInput(const GameInput *input, GameData *data);

void Main_Update(GameData *data);
void RequestGameStateTransition(GameData *data, const GameState nextState);

#endif

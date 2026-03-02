#ifndef GUARD_STATE_MANAGER_H
#define GUARD_STATE_MANAGER_H

#include "context.h"

typedef void (*InitFunc)(GameEngine *eng, GameData *data);
typedef void (*CleanupFunc)(GameEngine *eng, GameData *data);

void StateManager(GameEngine *eng, GameData *data);
void CleanupCurrentState(GameEngine *eng, GameData *data);

#endif

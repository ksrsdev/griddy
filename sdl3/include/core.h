#ifndef GUARD_CORE_H
#define GUARD_CORE_H

#include "context.h"

typedef void (*InitFunc)(GameData *data);
typedef void (*CleanupFunc)(GameData *data);

void CoreTick(const GameInput *input, GameData *data);


#endif

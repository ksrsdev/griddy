#ifndef GUARD_CORE_H
#define GUARD_CORE_H

#include "context.h"

typedef void (*CleanupFunc)(GameData *data);

void Core_Tick(const GameInput *input, GameData *data);


#endif

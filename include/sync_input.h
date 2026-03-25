#ifndef GUARD_SYNC_INPUT_H
#define GUARD_SYNC_INPUT_H

#include "context.h"

void Main_SyncInput(const GameInput *input, GameData *data);
u8 GetPaddingForWindow(const s32 windowWidth);

#endif

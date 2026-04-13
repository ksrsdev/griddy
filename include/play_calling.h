#ifndef GUARD_PLAY_CALLING_H
#define GUARD_PLAY_CALLING_H

#include "context.h"

void PlayCalling_Init(GameEngine *eng, GameData *data);
void PlayCalling_Cleanup(GameEngine *eng, GameData *data);
void PlayCalling_Update(GameData *data);
void PlayCalling_Render(const GameEngine *eng, const GameData *data);

#endif

#ifndef GUARD_INTRO_H
#define GUARD_INTRO_H

#include "context.h"

void IntroInit(GameEngine *eng, GameData *data);
void IntroTick(const GameInput *input, GameData *data);
void IntroRender(const GameEngine *eng, const GameData *data);
void IntroCleanup(GameEngine *eng, GameData *data);

#endif

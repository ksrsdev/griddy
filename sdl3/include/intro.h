#ifndef GUARD_INTRO_H
#define GUARD_INTRO_H

#include "context.h"

void Intro_Update(const GameInput *input, GameData *data);
void Intro_Render(const GameEngine *eng, const GameData *data);
void Intro_Init(GameEngine *eng, GameData *data);
void Intro_Cleanup(GameEngine *eng, GameData *data);

#endif

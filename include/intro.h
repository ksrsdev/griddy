#ifndef GUARD_INTRO_H
#define GUARD_INTRO_H

#include "context.h"
#include "state_data.h"
#include "types.h"

void Intro_Init(GameEngine *eng, GameData *data);
void Intro_Cleanup(GameEngine *eng, GameData *data);
void Intro_Update(const GameInput *input, GameData *data);
void Intro_Render(const GameEngine *eng, const GameData *data);

typedef void (*IntroAnimFunc)(IntroData *introData, const Vector2 screnSize, const uint64_t deltaTime);

#endif

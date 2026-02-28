#ifndef GUARD_INTRO_H
#define GUARD_INTRO_H

#include "context.h"
#include "types.h"

void Intro_Init(GameEngine *eng, GameData *data);
void Intro_Cleanup(GameEngine *eng, GameData *data);
void Intro_Update(const GameInput *input, GameData *data);
void Intro_Render(const GameEngine *eng, const GameData *data);

typedef void (*IntroAnimFunc)(SDL_FRect destRect, const Vector2 screnSize, const uint64_t deltaTime, double angle);

#endif

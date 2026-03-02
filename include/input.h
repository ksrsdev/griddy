#ifndef GUARD_INPUT_H
#define GUARD_INPUT_H

#include <SDL3/SDL.h>

#include "context.h"

void InputPollEvents(const GameEngine *eng, GameInput *input);

#endif

#ifndef GUARD_ERROR_H
#define GUARD_ERROR_H

#include "context.h"

typedef enum ErrorType {
	ERROR_TYPE_NONE,
	ERROR_TYPE_ALLOC,
	ERROR_TYPE_COUNT
} ErrorType

void Error_Init(GameEngine *eng, GameData *data);
void Error_Update(const GameInput *input, GameData *data);
void Error_Render(const GameEngine *eng, const GameData *data);
void Error_Cleanup(GameEngine *eng, GameData *data);

#endif

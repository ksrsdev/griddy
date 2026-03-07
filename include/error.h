#ifndef GUARD_ERROR_H
#define GUARD_ERROR_H

#include "context.h"
#include "error_code.h"

void Error_Init(GameEngine *eng, GameData *data);
void Error_Update(GameData *data);
void Error_Render(const GameEngine *eng, const GameData *data);
void Error_Cleanup(GameEngine *eng, GameData *data);

void Error_Alert(GameData *data, const ErrorCode errorCode, const char *errorMsg);

#endif

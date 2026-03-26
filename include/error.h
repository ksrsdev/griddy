#ifndef GUARD_ERROR_H
#define GUARD_ERROR_H

#include "context.h"
#include "error_code.h"

typedef enum {
	ERROR_UI_NONE,
	ERROR_UI_TITLE,
	ERROR_UI_ERROR_MSG,
	ERROR_UI_OK_BUTTON,
	ERROR_UI_COUNT
} ErrorUIElement;

void Error_Init(GameEngine *eng, GameData *data);
void Error_Update(GameData *data);
void Error_Render(const GameEngine *eng, const GameData *data);
void Error_Cleanup(GameEngine *eng, GameData *data);

void Error_Alert(GameData *data, const ErrorCode errorCode, const char *errorMsg);

#endif

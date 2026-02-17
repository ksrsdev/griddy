#include "global.h"
#include "error.h"
#include "state_manager.h"

#include "raylib.h"

#include <stdio.h>

static const ErrorDefinition ErrorTable[ERROR_COUNT] = {
	[ERROR_NONE] = { "ERROR NONE: ErrorType oob - ctx: %s", true },
	[ERROR_GLOBAL_CTX] = { "ERROR GLOBAL CTX: Tried to access %s", true },
	[ERROR_FILE_W] = { "ERROR FILE: Could not open %s for w mode", false },
	[ERROR_FILE_R] = { "ERROR FILE: Could not open %s for r mode", false },
	[ERROR_PLAYER_GEN] =  { "ERROR PLAYER GEN: %s", false },
	[ERROR_ALLOCATION] = { "ERROR ALLOCATION: %s", false },
};

static void FormatErrorMessage(char *dest, size_t destSize, const char *temp, const char *msg)
{
	#pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wformat-nonliteral"
    snprintf(dest, destSize, temp, msg);
    #pragma GCC diagnostic pop
}

void TriggerError(ErrorType type, const char *message)
{
	ErrorDefinition def = ErrorTable[type];
	FormatErrorMessage(ctx.errorMsg, sizeof(ctx.errorMsg), def.template, message);
	ctx.isErrorFatal = def.isErrorFatal;
	StateManager_UpdateGameState(MAIN_GAME_STATE_ERROR);
}


void DrawErrorScreen(void) 
{
	//Clear
	ClearBackground(RED);
	//Draw Text Box
	//Draw OK button
	//Draw Text
	//Check Button Press
}

#include "button.h"
#include "global.h"
#include "error.h"
#include "state_manager.h"
#include "text.h"

#include "raylib.h"

#include <stdio.h>

static void Error_Init(void);
static void Error_InitButtons(void);
static void Error_DrawTextBox(void);
static void Error_DrawOkButton(void);
static void Error_CheckButtonPress(void);

Button bError_OkButton;
static bool errorInitialized = false;

static const ErrorDefinition ErrorTable[ERROR_COUNT] = {
	[ERROR_NONE] = { "ERROR NONE: ErrorType oob - ctx: %s", true },
	[ERROR_GLOBAL_CTX] = { "ERROR GLOBAL CTX: Tried to access %s", true },
	[ERROR_FILE_W] = { "ERROR FILE: Could not open %s for w mode", false },
	[ERROR_FILE_R] = { "ERROR FILE: Could not open %s for r mode", false },
	[ERROR_PLAYER_GEN] =  { "ERROR PLAYER GEN: %s", false },
	[ERROR_ALLOCATION] = { "ERROR ALLOCATION: %s", false },
};

static void Error_Init(void)
{
	Error_InitButtons();
}

static void Error_InitButtons(void)
{
	bError_OkButton = MakeButton("OK", GRAY);
}

static void Error_DrawOkButton(void)
{
	float screenWidth = (float)GetScreenWidth();
	float screenHeight = (float)GetScreenHeight();
	//define OK button dimensions
	bError_OkButton.rec.width = screenWidth / 8.0f;
	bError_OkButton.rec.height = screenHeight / 8.0f;
	bError_OkButton.rec.x = (screenWidth - bError_OkButton.rec.width) / 2.0f;
	bError_OkButton.rec.y = (screenHeight - (bError_OkButton.rec.height * 1.5f));
	DrawSingleButton(&bError_OkButton);
}

static void FormatErrorMessage(char *dest, size_t destSize, const char *temp, const char *msg)
{
	#pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wformat-nonliteral"
    snprintf(dest, destSize, temp, msg);
    #pragma GCC diagnostic pop
}

static void Error_DrawTextBox(void)
{
	float screenWidth = (float)GetScreenWidth();
	float screenHeight = (float)GetScreenHeight();
	Rectangle errorTextBox;
	errorTextBox.width = screenWidth / 2.0f;
	errorTextBox.height = screenHeight / 2.0f;
	errorTextBox.x = (screenWidth - errorTextBox.width) / 2.0f;
	errorTextBox.y = (screenHeight - errorTextBox.height) / 2.0f;
	DrawRectangleRec(errorTextBox, BLACK);
	const char *errorMsg = ctx.errorMsg;
	errorTextBox.width -= errorTextBox.width / 10.0f;
	errorTextBox.x = (screenWidth - errorTextBox.width) / 2.0f;
	DrawTextInBoxColor(errorMsg, &errorTextBox, &RED);
}

void TriggerError(ErrorType type, const char *message)
{
	ErrorDefinition def = ErrorTable[type];
	FormatErrorMessage(ctx.errorMsg, sizeof(ctx.errorMsg), def.template, message);
	ctx.isErrorFatal = def.isErrorFatal;
	if (!errorInitialized) {
		Error_Init();
	}
	StateManager_UpdateGameState(MAIN_GAME_STATE_ERROR);
}


static void Error_CheckButtonPress(void)
{
	if (CheckSingleButtonForButtonPress(&bError_OkButton)) {
		if (ctx.isErrorFatal) {
			ctx.gameRunning = false;
		} else {
			ctx.state = ctx.prevState;
			ctx.prevState = MAIN_GAME_STATE_NONE;
		}
	}
}

void DrawErrorScreen(void) 
{
	//Clear
	ClearBackground(RED);
	//Draw Text Box
	Error_DrawTextBox();
	//Draw OK button
	Error_DrawOkButton();
	//Check Button Press
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		Error_CheckButtonPress(); 
	}
}

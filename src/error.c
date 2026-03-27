#include "error.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL3/SDL.h>

#include "colors.h"
#include "context.h"
#include "init.h"
#include "render.h"
#include "state_data.h"
#include "text.h"
#include "update.h"
#include "ui.h"

static void Error_LocalErrorFatal(const char *msg);
static void Error_LoadUIStrings(const GameData *data);
static void Error_LoadUIData(GameEngine *eng, GameData *data);
static void Error_ResizeLayout(ErrorData *data, const WindowState *window);
static void Error_CreateTextures(const GameEngine *eng, ErrorData *data);
static void Error_RecreateTexturesAfterResize(const GameEngine *eng, const GameData *data);
static void Error_CheckButtonHighlight(UIData *uiData, const FVector2 mousePos);
static ErrorUIElement  Error_CheckButtonClick(UIData *uiData, const FVector2 mousePos);
static bool IsErrorCodeFatal(ErrorCode errorCode);
static void Error_ExitButton_OnClick(GameData *data);
static void Error_ReturnButton_OnClick(GameData *data);


//   ***   FUNCTION DEFINITIONS   ***   
//


// *** INIT AND DEINIT   ***

void Error_Init(GameEngine *eng, GameData *data)
{
	//Create error data
	data->stateData = calloc(1, sizeof(ErrorData));
	if (data->stateData == NULL) {
		//error.c errors are fatal
		Error_LocalErrorFatal("errorData Did not Allocate");
		data->isRunning = false;
		return;
	}

	Error_LoadUIStrings(data);

	Error_LoadUIData(eng, data);
}

void Error_Cleanup(GameEngine *eng, GameData *data)
{
	(void)eng;

	ErrorData *errorData = data->stateData;

	for (int i = ERROR_UI_NONE + 1; i < ERROR_UI_COUNT; i++) {
		UIData *ui = &errorData->uiData[i];
		if (ui->texture) {
			SDL_DestroyTexture(ui->texture);
			ui->texture = NULL;
		}
	}

	Deinit_StateData(&data->stateData);
}

//   ***   UPDATE   ***

void Error_Update(GameData *data)
{
	ErrorData *errorData = data->stateData;
	
	if (data->window.resized) {
		errorData->texturesNeedResizing = true;
		Error_ResizeLayout(errorData, &data->window);
	}

	if (data->mouse.moved) {
		Error_CheckButtonHighlight(errorData->uiData, data->mouse.pos);
	}

	if (data->mouse.left.wasPressed) {
		s32 elementClicked = Error_CheckButtonClick(errorData->uiData, data->mouse.pos);
		if (elementClicked == ERROR_UI_OK_BUTTON) {
			UIData dataClicked = errorData->uiData[elementClicked];
			if (dataClicked.onClick) {
				OnClick onClick = errorData->uiData[ERROR_UI_OK_BUTTON].onClick;
				onClick(data);
			}
		}
	}
}

//   ***   RENDER   ***

void Error_Render(const GameEngine *eng, const GameData *data)
{
	//local pointers
	ErrorData *errorData = data->stateData;

	//Check resize texture after resize
	if (errorData->texturesNeedResizing) {
		u64 currTime = SDL_GetTicks();
		if (currTime - data->window.timeResized > 100) {
			Error_RecreateTexturesAfterResize(eng, data);
		}
	}

	//Red BG
	Render_SetDrawColor(eng->renderer, COLOR_RED);
	SDL_RenderClear(eng->renderer);

	//UI Elements
	for (s32 i = ERROR_UI_NONE + 1; i < ERROR_UI_COUNT; i++) {
		UI_RenderUIElement(eng, &errorData->uiData[i]);
	}
}

//   ***   MISC   ***

static void Error_LocalErrorFatal(const char *msg)
{
	printf("ERROR: %s\nerror.c error is fatal!\n", msg);
}

void Error_Alert(GameData *data, const ErrorCode errorCode, const char *errorMsg)
{
	data->errorCode = errorCode;
	snprintf(data->errorMsg, sizeof(data->errorMsg), "%s", errorMsg);

	RequestGameStateTransition(data, GAME_STATE_ERROR);
}

static void Error_CreateTextures(const GameEngine *eng, ErrorData *data)
{
	for (u8 i = ERROR_UI_NONE + 1; i < ERROR_UI_COUNT; i++) {
		data->uiData[i].texture = Text_CreateUITexture(eng, data->uiStrings[i], &data->uiData[i]);
	}
}

static void Error_LoadUIStrings(const GameData *data)
{
	ErrorData *errorData = data->stateData;

	errorData->uiStrings[ERROR_UI_TITLE] = "ERROR";

	if (data->errorMsg[0] != '\0') {
		errorData->uiStrings[ERROR_UI_ERROR_MSG] = data->errorMsg;
	} else {
		errorData->uiStrings[ERROR_UI_ERROR_MSG] = "Error Msg not found.";
	}

	errorData->uiStrings[ERROR_UI_OK_BUTTON] = "OK";
}

static void Error_LoadUIData(GameEngine *eng, GameData *data)
{
	ErrorData *errorData = data->stateData;

	//Set data first so we can use it later (type needed for CreateTexture)
	
	//#Title
	//type
	errorData->uiData[ERROR_UI_TITLE].type = UI_TYPE_TEXT;
	//fg
	errorData->uiData[ERROR_UI_TITLE].fg = COLOR_BLACK;

	
	//#Message Box
	//type
	errorData->uiData[ERROR_UI_ERROR_MSG].type = UI_TYPE_TEXT_WRAPPED;
	//fg
	errorData->uiData[ERROR_UI_ERROR_MSG].fg = COLOR_RED;
	//bg
	errorData->uiData[ERROR_UI_ERROR_MSG].bg = COLOR_BLACK;
	errorData->uiData[ERROR_UI_ERROR_MSG].hasBackground = true;

	
	//#Ok Button
	//type
	errorData->uiData[ERROR_UI_OK_BUTTON].type = UI_TYPE_BUTTON;
	//fg
	errorData->uiData[ERROR_UI_OK_BUTTON].fg = COLOR_RED;
	//bg
	errorData->uiData[ERROR_UI_OK_BUTTON].bg = COLOR_BLACK;
	errorData->uiData[ERROR_UI_OK_BUTTON].hasBackground = true;
	errorData->uiData[ERROR_UI_OK_BUTTON].outlineColor = COLOR_WHITE;
	//onClick
	if (IsErrorCodeFatal(data->errorCode)) {
		errorData->uiData[ERROR_UI_OK_BUTTON].onClick = Error_ExitButton_OnClick;
	} else {
		errorData->uiData[ERROR_UI_OK_BUTTON].onClick = Error_ReturnButton_OnClick;
	}
	
	//Set Layouts - keep generalized so we can re-call it when the window is resized!
	Error_ResizeLayout(errorData, &data->window);

	//Initial check mouse pos for button highlights
	Error_CheckButtonHighlight(errorData->uiData, data->mouse.pos);

	Error_CreateTextures(eng, errorData);
}

static void Error_ResizeLayout(ErrorData *data, const WindowState *window)
{
	float wX, wY;
	wX = (float)window->size.x;
	wY = (float)window->size.y;

	//Title
	data->uiData[ERROR_UI_TITLE].destRect.w = wX * 0.5f;
	data->uiData[ERROR_UI_TITLE].destRect.h = wY * 0.2f;
	data->uiData[ERROR_UI_TITLE].destRect.x = (wX - data->uiData[ERROR_UI_TITLE].destRect.w) * 0.5f;
	data->uiData[ERROR_UI_TITLE].destRect.y = wY * 0.1f;
	
	//messageDestRec = big text box
	data->uiData[ERROR_UI_ERROR_MSG].destRect.w = wX * 0.75f;
	data->uiData[ERROR_UI_ERROR_MSG].destRect.h = wY * 0.3f;
	data->uiData[ERROR_UI_ERROR_MSG].destRect.x = (wX - data->uiData[ERROR_UI_ERROR_MSG].destRect.w) * 0.5f;
	data->uiData[ERROR_UI_ERROR_MSG].destRect.y = data->uiData[ERROR_UI_TITLE].destRect.y + data->uiData[ERROR_UI_TITLE].destRect.h + (wY * 0.1f);
	
	
	//okButtonData
	data->uiData[ERROR_UI_OK_BUTTON].destRect.w = wX * 0.25f;
	data->uiData[ERROR_UI_OK_BUTTON].destRect.h = wY * 0.10f;
	data->uiData[ERROR_UI_OK_BUTTON].destRect.x = (wX - data->uiData[ERROR_UI_OK_BUTTON].destRect.w) * 0.5f;
	data->uiData[ERROR_UI_OK_BUTTON].destRect.y = data->uiData[ERROR_UI_ERROR_MSG].destRect.y + data->uiData[ERROR_UI_ERROR_MSG].destRect.h + (wY * 0.1f);
	
}

static void Error_RecreateTexturesAfterResize(const GameEngine *eng, const GameData *data)
{
	//It's only the one texture (msg) but in general it should be done to all uiData of certain types I think

	ErrorData *errorData = data->stateData;

	//Destroy old texture
	SDL_DestroyTexture(errorData->uiData[ERROR_UI_ERROR_MSG].texture);
	
	//Error Msg
	//confirm data.errorMsg contains info
	const char *errorString = data->errorMsg;
	if (errorString[0] == '\0') {
		errorString = "Error Msg not found.";
	}

	//Create new texture for new layout
	errorData->uiData[ERROR_UI_ERROR_MSG].texture = Text_CreateUITexture(eng, errorString, &errorData->uiData[ERROR_UI_ERROR_MSG]);
	if (errorData->uiData[ERROR_UI_ERROR_MSG].texture == NULL) {
		Error_LocalErrorFatal("Failed to create: ErrorMsg Texture");
	}

	//Clear flag
	errorData->texturesNeedResizing = false;
}

static void Error_CheckButtonHighlight(UIData *uiData, const FVector2 mousePos)
{
	for (s32 i = ERROR_UI_NONE + 1; i < ERROR_UI_COUNT; i++) {
		UI_UpdateHover(&uiData[i], mousePos);
	}
}

static ErrorUIElement Error_CheckButtonClick(UIData *uiData, const FVector2 mousePos)
{
	for (s32 i = ERROR_UI_NONE + 1; i < ERROR_UI_COUNT; i++) {
		 if (UI_CheckClick(&uiData[i], mousePos)) {
			 return i;
		 }
	}
	return 0;
}

static bool IsErrorCodeFatal(ErrorCode errorCode)
{
	//TODO Make this intelligent
	if (errorCode == ERROR_ALLOC) {
		return true;
	} else {
		return false;
	}
}

static void Error_ExitButton_OnClick(GameData *data)
{
	data->isRunning = false;
}

static void Error_ReturnButton_OnClick(GameData *data)
{
	RequestGameStateTransition(data, data->prevState);
}

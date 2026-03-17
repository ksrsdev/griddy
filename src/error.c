#include "error.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL3/SDL.h>

#include "colors.h"
#include "context.h"
#include "render.h"
#include "state_data.h"
#include "state_resources.h"
#include "text.h"
#include "update.h"

static void Error_LocalErrorFatal(const char *msg);
static bool Error_LoadData(GameEngine *eng, GameData *data);
static bool Error_CreateTextures(const GameEngine *eng, ErrorData *data, const char *errorMsg);
static void Error_ResizeLayout(ErrorData *data, const WindowState *window);
static bool IsErrorCodeFatal(ErrorCode errorCode);
static void Error_ExitOnClick(GameData *data);
static void Error_ReturnOnClick(GameData *data);

#define ERROR_TITLE_ASPECT_RATIO 0.4f // 5:2

//   ***   FUNCTION DEFINITIONS   ***   

void Error_Init(GameEngine *eng, GameData *data)
{
	//Create error data
	data->stateData = calloc(1, sizeof(ErrorData));
	if (data->stateData == NULL) {
		//error.c errors are fatal
		Error_LocalErrorFatal("stateData Did not Allocate");
		data->isRunning = false;
		return;
	}

	//Load resources
	if (!Error_LoadData(eng, data)) {
		data->isRunning = false;
		return;
	}
}

void Error_Cleanup(GameEngine *eng, GameData *data)
{
	(void)eng;
	ErrorData *errorData = data->stateData;

	for (int i = ERROR_UI_NONE + 1; i < ERROR_UI_COUNT; i++) {
		if (errorData->uiData[i].texture != NULL) {
			SDL_DestroyTexture(errorData->uiData[i].texture);
		}
	}

	//free error datae
	if (data->stateData != NULL) {
		free(data->stateData);
		data->stateData = NULL;
	}
}

void Error_Update(GameData *data)
{
	ErrorData *errorData = data->stateData;
	if (data->window.resized) {
		Error_ResizeLayout(errorData, &data->window);
	}
}

void Error_Render(const GameEngine *eng, const GameData *data)
{
	//local pointers
	ErrorData *errorData = data->stateData;

	//Red BG
	Render_SetDrawColor(eng->renderer, COLOR_RED);
	SDL_RenderClear(eng->renderer);

	Render_UIElement(eng, &errorData->uiData[ERROR_UI_TITLE], errorData->uiData[ERROR_UI_TITLE].texture);
	
	//Black Text Box w/ Red error Msg
	Render_UIElement(eng, &errorData->uiData[ERROR_UI_ERROR_MSG], errorData->uiData[ERROR_UI_ERROR_MSG].texture);
	
	//Black OK button bg with Red OK text
	
}

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

static bool Error_CreateTextures(const GameEngine *eng, ErrorData *data, const char *errorMsg)
{
	//Title
	data->uiData[ERROR_UI_TITLE].texture = Text_CreateTextTexture(eng->renderer, eng->textEngine, eng->font, "ERROR");
	if (data->uiData[ERROR_UI_TITLE].texture == NULL) {
		Error_LocalErrorFatal("Failed to create: Title Texture");
		return false;
	}
	
	//Error Msg
	//confirm data.errorMsg contains info
	const char *errorString = errorMsg;
	if (!errorMsg || errorString[0] == '\0') {
		errorString = "Error Msg not found.";
	}

	//Error Msg
	data->uiData[ERROR_UI_ERROR_MSG].texture = Text_CreateTextTextureWithLineWrap(eng->renderer, eng->textEngine, eng->font, errorString, &data->uiData[ERROR_UI_ERROR_MSG].destRect);
	if (data->uiData[ERROR_UI_ERROR_MSG].texture == NULL) {
		Error_LocalErrorFatal("Failed to create: ErrorMsg Texture");
		return false;
	}
	
	//OK Button
	data->uiData[ERROR_UI_OK_BUTTON].texture = Text_CreateTextTexture(eng->renderer, eng->textEngine, eng->font, "OK");
	if (data->uiData[ERROR_UI_OK_BUTTON].texture == NULL) {
		Error_LocalErrorFatal("Failed to create: OK Texture");
		return false;
	}
	
	return true;
}

static bool Error_LoadData(GameEngine *eng, GameData *data)
{
	ErrorData *errorData = data->stateData;

	//Set Layouts - keep generalized so we can re-call it when the window is resized!
	Error_ResizeLayout(errorData, &data->window);

	if (!Error_CreateTextures(eng, errorData, data->errorMsg)) {
		return false;
	}

	//#Title
	//type
	errorData->uiData[ERROR_UI_TITLE].type = UI_TYPE_TEXT;
	//fg
	errorData->uiData[ERROR_UI_TITLE].fg = COLOR_BLACK;

	
	//#Message Box
	//type
	errorData->uiData[ERROR_UI_ERROR_MSG].type = UI_TYPE_TEXT;
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
	//onClick
	if (IsErrorCodeFatal(data->errorCode)) {
		errorData->uiData[ERROR_UI_OK_BUTTON].onClick = Error_ExitOnClick;
	} else {
		errorData->uiData[ERROR_UI_OK_BUTTON].onClick = Error_ReturnOnClick;
	}

	return true;

}

static void Error_ResizeLayout(ErrorData *data, const WindowState *window)
{
	float wX, wY;
	wX = (float)window->size.x;
	wY = (float)window->size.y;

	//Title
	data->uiData[ERROR_UI_TITLE].destRect.w = wX * 0.5f;
	//data->uiData[ERROR_UI_TITLE].destRect.h = data->uiData[ERROR_UI_TITLE].destRect.w * ERROR_TITLE_ASPECT_RATIO;
	data->uiData[ERROR_UI_TITLE].destRect.h = wY * 0.2f;
	data->uiData[ERROR_UI_TITLE].destRect.x = (wX - data->uiData[ERROR_UI_TITLE].destRect.w) * 0.5f;
	data->uiData[ERROR_UI_TITLE].destRect.y = wY * 0.1f;
	
	//messageDestRec = big text box
	data->uiData[ERROR_UI_ERROR_MSG].destRect.w = wX * 0.75f;
	data->uiData[ERROR_UI_ERROR_MSG].destRect.h = wY * 0.3f;
	data->uiData[ERROR_UI_ERROR_MSG].destRect.x = (wX - data->uiData[ERROR_UI_ERROR_MSG].destRect.w) * 0.5f;
	data->uiData[ERROR_UI_ERROR_MSG].destRect.y = data->uiData[ERROR_UI_TITLE].destRect.y + data->uiData[ERROR_UI_TITLE].destRect.h + (wY * 0.1f);
	
	
	//okButtonData
	
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

static void Error_ExitOnClick(GameData *data)
{
	data->isRunning = false;
}

static void Error_ReturnOnClick(GameData *data)
{
	RequestGameStateTransition(data, data->prevState);
}

#undef ERROR_TITLE_ASPECT_RATIO


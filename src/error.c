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
static bool Error_LoadResources(GameEngine *eng, const char *errorMsg);
static void Error_LoadData(GameData *data);
static void Error_ResizeLayout(ErrorData *data, const WindowState *window);
static bool IsErrorCodeFatal(ErrorCode errorCode);
static void Error_ExitOnClick(GameData *data);
static void Error_ReturnOnClick(GameData *data);

#define ERROR_TITLE_ASPECT_RATIO 2.50

//   ***   FUNCTION DEFINITIONS   ***   

void Error_Init(GameEngine *eng, GameData *data)
{
	//Create error resources
	eng->stateResources = calloc(1, sizeof(ErrorResources));
	if (eng->stateResources == NULL) {
		Error_LocalErrorFatal("stateResources Did not Allocate");
		data->isRunning = false;
		return;
	}

	//Create error data
	data->stateData = calloc(1, sizeof(ErrorData));
	if (data->stateData == NULL) {
		//error.c errors are fatal
		Error_LocalErrorFatal("stateData Did not Allocate");
		data->isRunning = false;
		return;
	}

	//Load resources
	if (!Error_LoadResources(eng, data->errorMsg)) {
		data->isRunning = false;
		return;
	}

	//Load Data
	Error_LoadData(data);
}

void Error_Cleanup(GameEngine *eng, GameData *data)
{
	ErrorResources *errorResources = eng->stateResources;

	for (int i = ERROR_UI_NONE + 1; i < ERROR_UI_COUNT; i++) {
		if (errorResources->textures[i] != NULL) {
			SDL_DestroyTexture(errorResources->textures[i]);
		}
	}

	//free error datae
	if (data->stateData != NULL) {
		free(data->stateData);
		data->stateData = NULL;
	}

	//free error resources
	if (eng->stateResources != NULL) {
		free(eng->stateResources);
		eng->stateResources = NULL;
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
	ErrorResources *errorResources = eng->stateResources;
	ErrorData *errorData = data->stateData;

	//Red BG
	Render_SetDrawColor(eng->renderer, COLOR_RED);
	SDL_RenderClear(eng->renderer);

	Render_UIElement(eng, &errorData->uiData[ERROR_UI_TITLE], errorResources->textures[ERROR_UI_TITLE]);
	
	//Black Text Box
	
	//Red Error Msg
	
	//Black button bg
	
	//Red OK text
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

static bool Error_LoadResources(GameEngine *eng, const char *errorMsg)
{
	ErrorResources *errorResources = eng->stateResources;

	//Title
	errorResources->textures[ERROR_UI_TITLE] = Text_CreateTextTexture(eng->renderer, eng->textEngine, eng->font, "ERROR");
	if (errorResources->textures[ERROR_UI_TITLE] == NULL) {
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
	errorResources->textures[ERROR_UI_ERROR_MSG] = Text_CreateTextTexture(eng->renderer, eng->textEngine, eng->font, errorString);
	if (errorResources->textures[ERROR_UI_ERROR_MSG] == NULL) {
		Error_LocalErrorFatal("Failed to create: ErrorMsg Texture");
		return false;
	}
	
	//OK Button
	errorResources->textures[ERROR_UI_OK_BUTTON] = Text_CreateTextTexture(eng->renderer, eng->textEngine, eng->font, "OK");
	if (errorResources->textures[ERROR_UI_OK_BUTTON] == NULL) {
		Error_LocalErrorFatal("Failed to create: OK Texture");
		return false;
	}
	
	return true;
}

static void Error_LoadData(GameData *data)
{
	ErrorData *errorData = data->stateData;

	//Set Layouts - keep generalized so we can re-call it when the window is resized!
	Error_ResizeLayout(errorData, &data->window);

	//So here setup the things that dont change: bg, fg, onClick, outlined etc

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

}

static void Error_ResizeLayout(ErrorData *data, const WindowState *window)
{
	float wX, wY;
	wX = (float)window->size.x;
	wY = (float)window->size.y;

	//Title
	data->uiData[ERROR_UI_TITLE].destRect.w = wX / 2.0f;
	data->uiData[ERROR_UI_TITLE].destRect.h = data->uiData[ERROR_UI_TITLE].destRect.w / (float)ERROR_TITLE_ASPECT_RATIO;
	data->uiData[ERROR_UI_TITLE].destRect.x = (wX - data->uiData[ERROR_UI_TITLE].destRect.w) / 2.0f;
	data->uiData[ERROR_UI_TITLE].destRect.y = wY / 10.0f;
	
	//messageDestRec = big text box
	
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


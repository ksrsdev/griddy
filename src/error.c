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
static void Error_InitLayout(ErrorData *data, const WindowState *window, const MouseState *mouse);
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
	
	if (!Error_LoadResources(eng, data->errorMsg)) {
		data->isRunning = false;
		return;
	}

	//load data
	ErrorData *errorData = data->stateData;
	
	if (IsErrorCodeFatal(data->errorCode)) {
		errorData->uiData[ERROR_UI_OK_BUTTON].onClick = Error_ExitOnClick;
	} else {
		errorData->uiData[ERROR_UI_OK_BUTTON].onClick = Error_ReturnOnClick;
	}

	//Initial placement and sizing for elements (check button highlight too)
	Error_InitLayout(errorData, &data->window, &data->mouse);

	//Check if button is highlighted
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

	//Black ERROR title
	Render_SetupSDFRenderState(eng, errorData->uiData[ERROR_UI_TITLE].fg, errorResources->textures[ERROR_UI_TITLE]);
	SDL_RenderTexture(eng->renderer, errorResources->textures[ERROR_UI_TITLE], NULL, &errorData->uiData[ERROR_UI_TITLE].destRect);
	Render_ResetRenderState(eng->renderer);
	
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


	errorResources->textures[ERROR_UI_TITLE] = Text_CreateTextTexture(eng->renderer, eng->textEngine, eng->font, "ERROR");
	if (errorResources->textures[ERROR_UI_TITLE] == NULL) {
		Error_LocalErrorFatal("Failed to create: Title Texture");
		return false;
	}
	
	////Title
	//errorResources->textObjects[ERROR_UI_TITLE] = TTF_CreateText(eng->textEngine, eng->font, "ERROR", 0);
	//if (!errorResources->textObjects[ERROR_UI_TITLE]) {
	//	Error_LocalErrorFatal("Failed to create: Title TextObj");
	//	return false;
	//}

	//errorResources->textures[ERROR_UI_TITLE] = CreateTextureFromText(eng->renderer, errorResources->textObjects[ERROR_UI_TITLE]);
	//if (!errorResources->textures[ERROR_UI_TITLE]) {
	//	Error_LocalErrorFatal("Failed to create: Title Texture");
	//	return false;
	//}
	
	//Error Msg
	//check data.errorMsg actually contains info
	const char *errorString = errorMsg;
	if (!errorMsg || errorString[0] == '\0') {
		errorString = "Error Msg not found.";
	}
	
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

static void Error_InitLayout(ErrorData *errorData, const WindowState *window, const MouseState *mouse)
{
	(void)mouse;

	Error_ResizeLayout(errorData, window);
	
	//check okButton highlight

	//TODO: un(void) mouse once you use it 

}

static void Error_ResizeLayout(ErrorData *data, const WindowState *window)
{
	float wX, wY;
	wX = (float)window->size.x;
	wY = (float)window->size.y;

	//titleDestRec
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


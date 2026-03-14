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
		errorData->okButtonData.onClick = Error_ExitOnClick;
	} else {
		errorData->okButtonData.onClick = Error_ReturnOnClick;
	}

	//Initial placement and sizing for elements (check button highlight too)
	Error_InitLayout(errorData, &data->window, &data->mouse);

	//Check if button is highlighted
}

void Error_Cleanup(GameEngine *eng, GameData *data)
{
	ErrorResources *errorResources = eng->stateResources;

	//Destroy Text Objects
	TTF_DestroyText(errorResources->title);
	TTF_DestroyText(errorResources->errorMsg);
	TTF_DestroyText(errorResources->okButtonText);

	//free error data
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
	Render_SetupSDFRenderState(eng, errorResources->title, errorResources->titleTexture);
	SDL_RenderTexture(eng->renderer, errorResources->titleTexture, NULL, &errorData->titleDestRect);
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
	
	//Title
	//Text_InitSDFTexture(eng, errorResources->title, errorResources->titleTexture, "ERROR", COLOR_BLACK);
	errorResources->title = TTF_CreateText(eng->textEngine, eng->font, "ERROR", 0);
	Text_SetColor(errorResources->title, COLOR_BLACK);
	errorResources->titleTexture = CreateTextureFromText(eng->renderer, errorResources->title);
	
	//Error Msg

	//check data.errorMsg actually contains info
	const char *errorString = errorMsg;
	if (errorString[0] == '\0') {
		errorString = "Error Msg not found.";
	}
	
	errorResources->errorMsg = TTF_CreateText(eng->textEngine, eng->font, errorString, 0);
	if (!errorResources->errorMsg) {
		Error_LocalErrorFatal("Failed to create: errorResources->errorMsg");
		return false;
	}
	Text_SetColor(errorResources->errorMsg, COLOR_RED);

	//TODO: Move this somewhere else
//	memset(errorMsg, 0, sizeof(errorMsg));

	//Button Text
	errorResources->okButtonText = TTF_CreateText(eng->textEngine, eng->font, "OK", 0);
	if (!errorResources->okButtonText) {
		Error_LocalErrorFatal("Failed to create: errorResources->okButtonText");
		return false;
	}
	Text_SetColor(errorResources->okButtonText, COLOR_RED);

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
	data->titleDestRect.w = wX / 2.0f;
	data->titleDestRect.h = data->titleDestRect.w / (float)ERROR_TITLE_ASPECT_RATIO;
	data->titleDestRect.x = (wX - data->titleDestRect.w) / 2.0f;
	data->titleDestRect.y = wY / 10.0f;
	
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


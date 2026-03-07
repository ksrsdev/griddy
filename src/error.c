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
#include "update.h"

static void Error_LocalErrorFatal(const char *msg);
static bool Error_LoadResources(GameEngine *eng, const char *errorMsg);
static bool IsErrorCodeFatal(ErrorCode errorCode);
static void Error_ExitOnClick(GameData *data);
static void Error_ReturnOnClick(GameData *data);

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

	//Check if button is highlighted
}

void Error_Cleanup(GameEngine *eng, GameData *data)
{
	(void)eng;
	(void)data;
	
	//local pointers
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
	(void)data;
}

void Error_Render(const GameEngine *eng, const GameData *data)
{
	(void)data;

	//local pointers
	ErrorResources *errorResources = eng->stateResources;

	//Red BG
	Render_SetDrawColor(eng->renderer, COLOR_RED);
	SDL_RenderClear(eng->renderer);

	//Black ERROR title
	TTF_DrawRendererText(errorResources->title, 100, 100);
	
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
	errorResources->title = TTF_CreateText(eng->textEngine, eng->font, "ERROR", 0);
	if (!errorResources->title) {
		Error_LocalErrorFatal("Failed to create: errorResources->title");
		return false;
	}
	Text_SetColor(errorResources->title, COLOR_BLACK);
	
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



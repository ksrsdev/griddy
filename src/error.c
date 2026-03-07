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

static bool IsErrorCodeFatal(ErrorCode errorCode);
static void Error_ExitOnClick(GameData *data);
static void Error_ReturnOnClick(GameData *data);

//   ***   FUNCTION DEFINITIONS   ***   

void Error_Init(GameEngine *eng, GameData *data)
{
	(void)eng;
	(void)data;

	//Create error resources
	eng->stateResources = calloc(1, sizeof(ErrorResources));
	if (eng->stateResources == NULL) {
		//ERROR
		return;
	}

	//Create error data
	data->stateData = calloc(1, sizeof(ErrorData));
	if (data->stateData == NULL) {
		//ERROR
		return;
	}
	
	//local pointers
	ErrorData *introData = data->stateData;
	ErrorResources *introResources = eng->stateResources;
	
	//load resources

	//Title
	introResources->title = TTF_CreateText(eng->textEngine, eng->font, "ERROR", 0);
	Text_SetColor(introResources->title, COLOR_BLACK);

	//Error Msg

	//check data.errorMsg actually contains info
	const char *errorString = data->errorMsg;
	if (errorString[0] == '\0') {
		errorString = "Error Msg not found.";
	}
	
	introResources->errorMsg = TTF_CreateText(eng->textEngine, eng->font, errorString, 0);
	Text_SetColor(introResources->errorMsg, COLOR_RED);

	//clear data.errorMsg for next use (TODO clear this in the AlertError func too)
	memset(data->errorMsg, 0, sizeof(data->errorMsg));

	//Button Text
	introResources->okButtonText = TTF_CreateText(eng->textEngine, eng->font, "OK", 0);
	Text_SetColor(introResources->okButtonText, COLOR_RED);

	//load data
	if (IsErrorCodeFatal(data->errorCode)) {
		introData->okButtonData.onClick = Error_ExitOnClick;
	} else {
		introData->okButtonData.onClick = Error_ReturnOnClick;
	}
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

void Error_Update(const GameInput *input, GameData *data)
{
	(void)input;
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

void Error_Alert(GameData *data, const ErrorCode errorCode, const char *errorMsg)
{
	data->errorCode = errorCode;
	snprintf(data->errorMsg, sizeof(data->errorMsg), "%s", errorMsg);

	RequestGameStateTransition(data, GAME_STATE_ERROR);
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


#include "error.h"

#include "context.h"

static void Error_OkButtonOnClick(GameData *data);

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
	IntroData *introData = data->stateData;
	IntroResources *introResources = eng->stateResources;
	
	//load resources

	//Title
	introResources->title = TTF_CreateText(eng->textEngine, eng->font, "ERROR", 0);
	Text_SetColor(introResources->title, COLOR_BLACK);

	//Error Msg

	//check data.errorMsg actually contains info
	const char *errorMsg = data.errorMsg;
	if (errorMsg[0] == '\0') {
		errorMsg = "Error Msg not found.";
	}
	
	introResources->errorMsg = TTF_CreateText(eng->textEngine, eng->font, errorMsg, 0);
	Text_SetColor(introResources->errorMsg, COLOR_RED);

	//clear data.errorMsg for next use (TODO clear this in the AlertError func too)
	memset(data.errorMsg, 0, sizeof(data.errorMsg));

	//Button Text
	introResources->okButtonText = TTF_CreateText(eng->textEngine, eng->font, "OK", 0);
	Text_SetColor(introResources->okButtonText, COLOR_RED);

	//load data
	if (IsErrorTypeFatal(introData->errorType)) {
		introData->okButtonData.onClick = Error_ExitOnClick;
	} else {
		introData->okButtonData.onClick = Error_ReturnOnClick;
	}
}

void Error_Cleanup(GameEngine *eng, GameData *data)
{
	(void)eng;
	(void)data;

	//free error data
	free(data->stateData)

	//free error resources
}

void Error_Update(const GameInput *input, GameData *data)
{
	(void)input;
	(void)data;
}

void Error_Render(const GameEngine *eng, const GameData *data)
{
	//A red background. Black ERROR title. A black text box with red error msg details and a black button with red text OK
	(void)eng;
	(void)data;
}

static void Error_OkButtonOnClick(GameData *data)
{
	
	RequestGameStateTransition

}


#include "team_select.h"

#include <stdlib.h>

#include "colors.h"
#include "context.h"
#include "init.h"
#include "render.h"
#include "state_data.h"
#include "update.h"

static void TeamSelect_LoadUIStrings(const GameData *data);
static void TeamSelect_LoadUIData(const GameEngine *eng, const GameData *data);

static void TeamSelect_ResizeLayout(TeamSelectData *data, const Vector2 windowSize);
//static void TeamSelect_CreateTextures(const GameEngine *eng, TeamSelectData *data);

static void TeamSelect_CheckButtonHighlight(UIData *uiDat, const FVector2 mousePos);

const char * TeamSelect_GetTitleText(const TeamAssignment *assignment);
//const char * TeamSelect_GetInfoText(const TeamAssignment *assignment);

//   ###   INIT   ###
void TeamSelect_Init(GameEngine *eng, GameData *data)
{
	data->stateData = calloc(1, sizeof(TeamSelectData));
	if (data->stateData == NULL) {
		//error.c errors are fatal
		Error_Alert(data, ERROR_ALLOC, "teamSelectData failed calloc()");
		return;
	}
	
	//remove (void) above when you actually add this stuff
	TeamSelect_LoadUIStrings(data);

	TeamSelect_LoadUIData(eng, data);

}

void TeamSelect_Cleanup(GameEngine *eng, GameData *data)
{
	(void)eng;

	TeamSelectData *teamSelectData = data->stateData;

	for (s32 i = TEAM_SELECT_UI_START; i < TEAM_SELECT_UI_END; i++) {

		UIData *uiData = &teamSelectData->uiData[i];
		if (uiData->texture) {
			SDL_DestroyTexture(uiData->texture);
			uiData->texture = NULL;
		}
	}

	Deinit_StateData(&data->stateData);
}

//   ###   UPDATE   ###
void TeamSelect_Update(GameData *data) 
{
	TeamSelectData *teamSelectData = data->stateData;
	
	if (data->window.resized) {
		TeamSelect_ResizeLayout(teamSelectData, data->window.size);
	}
	
	if (data->mouse.moved) {
		TeamSelect_CheckButtonHighlight(teamSelectData->uiData, data->mouse.pos);
	}

}

//   ###   RENDER   ###
void TeamSelect_Render(const GameEngine *eng, const GameData *data)
{
	TeamSelectData *teamSelectData = data->stateData;
	
	//Clear White
	Render_SetDrawColor(eng->renderer, COLOR_WHITE);
	SDL_RenderClear(eng->renderer);

	//UI Elements
	for (s32 i = TEAM_SELECT_UI_START; i < OPTIONS_MENU_UI_END; i++) {
		UIData *uiData = &teamSelectData->uiData[i];
		UI_RenderUIElement(eng, uiData);
	}

}

static void TeamSelect_LoadUIStrings(const GameData *data)
{
	TeamSelectData *teamSelectData = data->stateData;

	//Title
	const char *titleText = TeamSelect_GetTitleText(&data->teamAssignment);
	teamSelectData->uiStrings[TEAM_SELECT_UI_TITLE] = titleText;

	//Info Box
//	const char *infoText = TeamSelect_GetInfoText(&data->teamAssignment);

	const char *infoText = "TO BE COMPLETED :D";
	teamSelectData->uiStrings[TEAM_SELECT_UI_INFO_BOX] = infoText;

	//Buttons
	teamSelectData->uiStrings[TEAM_SELECT_UI_RANDOM] = "RANDOM";
	teamSelectData->uiStrings[TEAM_SELECT_UI_BLACK]  = "BLACK";
	teamSelectData->uiStrings[TEAM_SELECT_UI_WHITE]  = "WHITE";
	teamSelectData->uiStrings[TEAM_SELECT_UI_GREEN]  = "GREEN";
	teamSelectData->uiStrings[TEAM_SELECT_UI_RED]    = "RED";
	teamSelectData->uiStrings[TEAM_SELECT_UI_PINK]   = "PINK";
	teamSelectData->uiStrings[TEAM_SELECT_UI_BROWN]  = "BROWN";
	teamSelectData->uiStrings[TEAM_SELECT_UI_YELLOW] = "YELLOW";
	teamSelectData->uiStrings[TEAM_SELECT_UI_ORANGE] = "ORANGE";
	teamSelectData->uiStrings[TEAM_SELECT_UI_BLUE]   = "BLUE";
	
	teamSelectData->uiStrings[TEAM_SELECT_UI_BACK]     = "BACK";
	teamSelectData->uiStrings[TEAM_SELECT_UI_PREVIEW]  = "VIEW ROSTER";
	teamSelectData->uiStrings[TEAM_SELECT_UI_CONTINUE] = "CONTINUE";
}

static void TeamSelect_LoadUIData(const GameEngine *eng, const GameData *data)
{
	(void)eng;
	(void)data;
}

static void TeamSelect_ResizeLayout(TeamSelectData *data, const Vector2 windowSize)
{
	(void)data;
	(void)windowSize;

}

//static void TeamSelect_CreateTextures(const GameEngine *eng, TeamSelectData *data)
//{
//	(void)eng;
//	(void)data;
//
//}

static void TeamSelect_CheckButtonHighlight(UIData *uiData, const FVector2 mousePos)
{
	(void)uiData;
	(void)mousePos;

}

const char * TeamSelect_GetTitleText(const TeamAssignment *assignment)
{
	if (assignment->player == TEAM_ID_NONE) {
		return "SELECT PLAYER TEAM";
	} else if (assignment->cpu == TEAM_ID_NONE) {
		return "SELECT CPU TEAM";
	} else {
		return "TEAM SELECT";
	}
}

//const char * TeamSelect_GetInfoText(const TeamAssignment *assignment)
//{
//
//}

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
	(void)data;
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

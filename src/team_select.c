#include "team_select.h"

#include <stdlib.h>

#include "colors.h"
#include "context.h"
#include "init.h"
#include "render.h"
#include "state_data.h"
#include "update.h"

static void TeamSelect_LoadUIStrings(const GameData *data);
static void TeamSelect_LoadInfoBoxStrings(TeamSelectData *data, const TeamAssignment *assignment);
static void TeamSelect_LoadInfoBoxStringsNoFocus(TeamSelectData *data);

static void TeamSelect_LoadUIData(const GameEngine *eng, const GameData *data);
static void TeamSelect_InitUIData(TeamSelectData *data);

static void TeamSelect_ResizeLayout(TeamSelectData *data, const Vector2 windowSize);
//static void TeamSelect_CreateTextures(const GameEngine *eng, TeamSelectData *data);

static void TeamSelect_CheckButtonHighlight(UIData *uiDat, const FVector2 mousePos);

const char * TeamSelect_GetTitleText(const TeamAssignment *assignment);
//const char * TeamSelect_GetInfoText(const TeamAssignment *assignment);

const SDL_Color sTeamButtonColors[TEAM_SELECT_UI_TEAM_BUTTON_ROW_2_END - TEAM_SELECT_UI_TEAM_BUTTON_ROW_1_START] = {
	COLOR_BLACK,
	COLOR_WHITE,
	COLOR_GREEN,
	COLOR_RED,
	COLOR_PINK,
	COLOR_YELLOW,
	COLOR_BROWN,
	COLOR_ORANGE,
	COLOR_BLUE
};

//   ###   INIT   ###
void TeamSelect_Init(GameEngine *eng, GameData *data)
{
	data->stateData = calloc(1, sizeof(TeamSelectData));
	if (data->stateData == nullptr) {
		//error.c errors are fatal
		Error_Alert(data, ERROR_ALLOC, "teamSelectData failed calloc()");
		return;
	}
	
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
			uiData->texture = nullptr;
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

//   ###   STRINGS   ###

static void TeamSelect_LoadUIStrings(const GameData *data)
{
	TeamSelectData *teamSelectData = data->stateData;

	//Title
	const char *titleText = TeamSelect_GetTitleText(&data->teamAssignment);
	teamSelectData->uiStrings[TEAM_SELECT_UI_TITLE] = titleText;

	//Info Box
//	const char *infoText = TeamSelect_GetInfoText(&data->teamAssignment);

	TeamSelect_LoadInfoBoxStrings(teamSelectData, &data->teamAssignment);


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
	
	teamSelectData->uiStrings[TEAM_SELECT_UI_BACK]     = "<-BACK";
	teamSelectData->uiStrings[TEAM_SELECT_UI_PREVIEW]  = "VIEW ROSTER";
	teamSelectData->uiStrings[TEAM_SELECT_UI_CONTINUE] = "CONTINUE->";
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

static void TeamSelect_LoadInfoBoxStrings(TeamSelectData *data, const TeamAssignment *assignment)
{
	if (assignment->preview == TEAM_ID_NONE) {
		TeamSelect_LoadInfoBoxStringsNoFocus(data);
		return;
	}

	//Load data from focus team data
	data->uiStrings[TEAM_SELECT_UI_INFO_BOX] = "FOCUS";

	//WIP - Should load strings via team data
}

static void TeamSelect_LoadInfoBoxStringsNoFocus(TeamSelectData *data)
{
	data->uiStrings[TEAM_SELECT_UI_INFO_BOX] = "NOFOCUS";

	//WIP - Should clear the other strings
}

//   ###   UIDATA   ###

static void TeamSelect_LoadUIData(const GameEngine *eng, const GameData *data)
{
	TeamSelectData *teamSelectData = data->stateData;

	TeamSelect_InitUIData(teamSelectData);

	//ON CLICK stuff when ready
	
	TeamSelect_ResizeLayout(teamSelectData, data->window.size);

	
}

//Just set the stuff that will always be true (type mostly)- this should only run via INIT
static void TeamSelect_InitUIData(TeamSelectData *data)
{
	UIData *uiData = nullptr;

	//Title
	uiData = &data->uiData[TEAM_SELECT_UI_TITLE];

	uiData->type = UI_TYPE_TEXT;
	uiData->fg   = COLOR_BLACK;

	//Mass init all team buttons - they will be fixed later...?
	for (s32 i = TEAM_SELECT_UI_TEAM_BUTTON_ROW_1_START; i < TEAM_SELECT_UI_TEAM_BUTTON_ROW_2_END; i++) {

		//Figure out our index for the team colors - remember to offset by 1 for random!
		s32 j = i - TEAM_SELECT_UI_TEAM_BUTTON_ROW_1_START - 1;
		
		uiData = &data->uiData[i];

		uiData->type = UI_TYPE_BUTTON;
		uiData->hasBackground = true;

		//Random button - remember this is just the init. It starts red anyways...I think
		if (j < 0) {
			uiData->fg = COLOR_BLACK;
			uiData->bg = COLOR_RED;
			uiData->outlineColor = COLOR_BLACK;
		} else { //ALl other buttons get bg from index
			uiData->bg = sTeamButtonColors[j];
			if (sTeamButtonColors[j] == COLOR_BLACK) { //Black needs opposite fg and red highlight
				uiData->fg = COLOR_WHITE;
				uiData->outlineColor = COLOR_RED;
			} else { //All other buttons follow pattern
				uiData->fg = COLOR_BLACK;
				uiData->outlineColor = COLOR_BLACK;
			}
		}
	}

	//Info Box
	uiData = &data->uiData[TEAM_SELECT_UI_INFO_BOX];

	uiData->type         = UI_TYPE_INFO_BOX;
	uiData->fg           = COLOR_BLACK;
	uiData->bg           = COLOR_BLACK;
	uiData->outlineColor = COLOR_BLACK;
	uiData->outlined     = true;

	//Info Box Members
	for (s32 i = TEAM_SELECT_UI_INFO_BOX_MEMBER_START; i < TEAM_SELECT_UI_INFO_BOX_MEMBER_END; i++) {
		uiData = &data->uiData[i];

		uiData->type = UI_TYPE_TEXT;
		uiData->fg = COLOR_BLACK;
	}
	
	//Back Button
	uiData = &teamSelectData->uiData[TEAM_SELECT_UI_BACK];
	UI_SetupBackButton(uiData);
	
	//View Roster 
	uiData = &teamSelectData->uiData[TEAM_SELECT_UI_PREVIEW];
	UI_SetupDefaultButton(uiData);
	
	//Continue
	uiData = &teamSelectData->uiData[TEAM_SELECT_UI_PREVIEW];
	UI_SetupButton(uiData, COLOR_BLACK, COLOR_GREEN);


}

static void TeamSelect_ResizeLayout(TeamSelectData *data, const Vector2 windowSize)
{
	f32 wX = (f32)windowSize.x;
	f32 wY = (f32)windowSize.y;

	SDL_FRect *dest = nullptr;

	//Title
	dest = &data->uiData[TEAM_SELECT_UI_TITLE].destRect;

	dest->w = wX * 0.5f;
	dest->h = wY * 0.1f;
	dest->x = (wX - dest->w) * 0.5f;
	dest->y = wY * 0.1f;

	//Team Button Row 1
	//Team Button Row 2
	//Info Box
	//Info Box Members
	//Back
	//Preview
	//Continue

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

//const char * TeamSelect_GetInfoText(const TeamAssignment *assignment)
//{
//
//}

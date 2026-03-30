#include "team_select.h"

#include <stdlib.h>
#include <stdio.h>

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

static void TeamSelect_ResizeLayout(UIData *data, const Vector2 windowSize);
static void TeamSelect_ResizeInfoBoxMembers(UIData *data);

static void TeamSelect_CreateTextures(const GameEngine *eng, TeamSelectData *data);

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
		TeamSelect_ResizeLayout(teamSelectData->uiData, data->window.size);
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
	for (s32 i = TEAM_SELECT_UI_START; i < TEAM_SELECT_UI_END; i++) {
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
	
	TeamSelect_ResizeLayout(teamSelectData->uiData, data->window.size);

	TeamSelect_CheckButtonHighlight(teamSelectData->uiData, data->mouse.pos);
	
	TeamSelect_CreateTextures(eng, teamSelectData);
	
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
			if (Colors_AreEqual(sTeamButtonColors[j], COLOR_BLACK)) { //white fg & red highlight
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

	uiData->type          = UI_TYPE_TEXT;
	uiData->fg            = COLOR_BLUE;
	uiData->bg            = COLOR_WHITE;
	uiData->hasBackground = true;
	uiData->outlineColor  = COLOR_BLACK;
	uiData->outlined      = true;

	//Info Box Members
	for (s32 i = TEAM_SELECT_UI_INFO_BOX_MEMBER_START; i < TEAM_SELECT_UI_INFO_BOX_MEMBER_END; i++) {
		uiData = &data->uiData[i];

		uiData->type = UI_TYPE_TEXT;
		uiData->fg = COLOR_BLACK;
	}
	
	//Back Button
	uiData = &data->uiData[TEAM_SELECT_UI_BACK];
	UI_SetupBackButton(uiData);
	
	//View Roster 
	uiData = &data->uiData[TEAM_SELECT_UI_PREVIEW];
	UI_SetupDefaultButton(uiData);
	
	//Continue
	uiData = &data->uiData[TEAM_SELECT_UI_CONTINUE];
	UI_SetupButton(uiData, COLOR_BLACK, COLOR_GREEN);


}

static void TeamSelect_ResizeLayout(UIData *data, const Vector2 windowSize)
{
	f32 wX = (f32)windowSize.x;
	f32 wY = (f32)windowSize.y;

	SDL_FRect *dest = nullptr;

	//Title
	dest = &data[TEAM_SELECT_UI_TITLE].destRect;

	dest->w = wX * 0.5f;
	dest->h = wY * 0.20f;
	dest->x = (wX - dest->w) * 0.5f;
	dest->y = wY * 0.05f;

	//TEAM BUTTONS
	
	SDL_FRect buttonRowArea = {};
	buttonRowArea.x = wX * 0.1f;
	buttonRowArea.y = wY * 0.25f;
	buttonRowArea.w = wX * 0.8f;
	buttonRowArea.h = wY * 0.1f;

	s32 numButtonsInRow = TEAM_SELECT_UI_TEAM_BUTTON_ROW_1_END - TEAM_SELECT_UI_TEAM_BUTTON_ROW_1_START;
	s32 numSpaces = numButtonsInRow - 1;

	f32 spacesW = (buttonRowArea.w * 0.2f) / (f32)numSpaces;
	
	//Team Button Row 1
	for (s32 i = TEAM_SELECT_UI_TEAM_BUTTON_ROW_1_START; i < TEAM_SELECT_UI_TEAM_BUTTON_ROW_1_END; i++) {

		dest = &data[i].destRect;

		dest->h = buttonRowArea.h;
		dest->y = buttonRowArea.y;
		dest->w = (buttonRowArea.w * 0.8f) / (f32)numButtonsInRow;
		if (i == TEAM_SELECT_UI_TEAM_BUTTON_ROW_1_START) {
			dest->x = buttonRowArea.x;
		} else {
			dest->x = data[i - 1].destRect.x + dest->w + spacesW;
		}
	}

	//Adjust Y value for 2 row
	buttonRowArea.y = wY * 0.40f;

	//Team Button Row 2
	for (s32 i = TEAM_SELECT_UI_TEAM_BUTTON_ROW_2_START; i < TEAM_SELECT_UI_TEAM_BUTTON_ROW_2_END; i++) {

		dest = &data[i].destRect;

		dest->h = buttonRowArea.h;
		dest->y = buttonRowArea.y;
		dest->w = (buttonRowArea.w * 0.8f) / (f32)numButtonsInRow;
		if (i == TEAM_SELECT_UI_TEAM_BUTTON_ROW_2_START) {
			dest->x = buttonRowArea.x;
		} else {
			dest->x = data[i - 1].destRect.x + dest->w + spacesW;
		}
	}

	//Info Box
	dest = &data[TEAM_SELECT_UI_INFO_BOX].destRect;

	dest->x = wX * 0.1f;
	dest->y = wY * 0.55f;
	dest->w = wX * 0.8f;
	dest->h = wY * 0.25f;

	dest = nullptr;

	//Info Box Members
	TeamSelect_ResizeInfoBoxMembers(data);

	//Back
	data[TEAM_SELECT_UI_BACK].destRect = UI_GetBackButtonDestRect(wX, wY);
	
	dest = &data[TEAM_SELECT_UI_BACK].destRect;

	//Overwrite Y value to make it match other bottom buttons (this will all be changed im sure)
	dest->y = wY - dest->h - (wY * 0.05f);

	//Preview
	dest = &data[TEAM_SELECT_UI_PREVIEW].destRect;
	
	dest->w = wX * 0.1f;
	dest->h = wY * 0.1f;
	dest->x = (wX * 0.5f) - (dest->w * 0.5f);
	dest->y = wY - dest->h - (wY * 0.05f);

	//Continue
	dest = &data[TEAM_SELECT_UI_CONTINUE].destRect;
	
	dest->w = wX * 0.1f;
	dest->h = wY * 0.1f;
	dest->x = (wX) - (dest->w) - (wX * 0.05f);
	dest->y = wY - dest->h - (wY * 0.05f);

}

static void TeamSelect_ResizeInfoBoxMembers(UIData *data)
{
	SDL_FRect infoBox = data[TEAM_SELECT_UI_INFO_BOX].destRect;
	
	f32 infoX = infoBox.x;
	f32 infoY = infoBox.y;
	f32 infoW = infoBox.w;
	f32 infoH = infoBox.h;

	SDL_FRect *dest = nullptr;

	//Title
	dest = &data[TEAM_SELECT_UI_INFO_BOX_TITLE].destRect;

	*dest = UI_GetTitleDestRect(infoW, infoH);
	dest->x += infoX;
	dest->y += infoY;

	//Desc
	dest = &data[TEAM_SELECT_UI_INFO_BOX_DESC].destRect;

	dest->w = infoW * 0.8f;
	dest->h = infoH * 0.1f;
	dest->x = (infoW - dest->w) * 0.5f;
	dest->y = infoH * 0.4f;

	//Pros
	dest = &data[TEAM_SELECT_UI_INFO_BOX_PROS].destRect;

	dest->w = infoW * 0.8f;
	dest->h = infoH * 0.1f;
	dest->x = (infoW - dest->w) * 0.5f;
	dest->y = infoH * 0.6f;

	//Cons
	dest = &data[TEAM_SELECT_UI_INFO_BOX_CONS].destRect;

	dest->w = infoW * 0.8f;
	dest->h = infoH * 0.1f;
	dest->x = (infoW - dest->w) * 0.5f;
	dest->y = infoH * 0.7f;

	//Off
	dest = &data[TEAM_SELECT_UI_INFO_BOX_OFF].destRect;

	dest->w = infoW * 0.4f;
	dest->h = infoH * 0.1f;
	dest->x = infoW * 0.05f;
	dest->y = infoH * 0.8f;
	
	//Def
	dest = &data[TEAM_SELECT_UI_INFO_BOX_DEF].destRect;

	dest->w = infoW * 0.4f;
	dest->h = infoH * 0.1f;
	dest->x = (infoW * 0.5f) + (infoW * 0.05f);
	dest->y = infoH * 0.8f;
}

static void TeamSelect_CreateTextures(const GameEngine *eng, TeamSelectData *data)
{
	for (s32 i = TEAM_SELECT_UI_START; i < TEAM_SELECT_UI_END; i++) {
		data->uiData[i].texture = Text_CreateUITexture(eng, data->uiStrings[i], &data->uiData[i]);
	}
}

static void TeamSelect_CheckButtonHighlight(UIData *uiData, const FVector2 mousePos)
{
	(void)uiData;
	(void)mousePos;

}

//const char * TeamSelect_GetInfoText(const TeamAssignment *assignment)
//{
//
//}

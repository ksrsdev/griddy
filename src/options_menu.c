#include "options_menu.h"

#include <stdlib.h>

#include "colors.h"
#include "context.h"
#include "init.h"
#include "render.h"
#include "state_data.h"

static void OptionsMenu_LoadUIStrings(const GameData *data);
static void OptionsMenu_LoadUIData(const GameEngine *eng, const GameData *data);

static void OptionsMenu_ResizeLayout(OptionsMenuData *data, const Vector2 windowSize, const u8 padding);
static void OptionsMenu_CheckButtonHighlight(UIData *uiDat, const FVector2 mousePos);
static void OptionsMenu_CreateTextures(const GameEngine *eng, OptionsMenuData *data);

//   ###   INIT   ###
void OptionsMenu_Init(GameEngine *eng, GameData *data) 
{
	(void)eng;
	data->stateData = calloc(1, sizeof(OptionsMenuData));
	if (data->stateData == NULL) {
		//error.c errors are fatal
		Error_Alert(data, ERROR_ALLOC, "opionsMenuData failed calloc()");
		return;
	}
	
	//remove (void) above when you actually add this stuff
	OptionsMenu_LoadUIStrings(data);

	OptionsMenu_LoadUIData(eng, data);

}

//   ###   CLEANUP   ###
void OptionsMenu_Cleanup(GameEngine *eng, GameData *data) 
{
	(void)eng;

	OptionsMenuData *optionsMenuData = data->stateData;

	for (s32 i = OPTIONS_MENU_UI_START; i < OPTIONS_MENU_UI_END; i++) {

		UIData *uiData = &optionsMenuData->uiData[i];
		if (uiData->texture) {
			SDL_DestroyTexture(uiData->texture);
			uiData->texture = NULL;
		}
	}

	Deinit_StateData(&data->stateData);
}

//   ###   UPDATE   ###
void OptionsMenu_Update(GameData *data) 
{
	(void)data;

	//remove (void) above when you actually add this stuff
}

//   ###   RENDER   ###
void OptionsMenu_Render(const GameEngine *eng, const GameData *data) 
{
	(void)data;

	//remove (void) above when you actually add this stuff

	//Clear White
	Render_SetDrawColor(eng->renderer, COLOR_WHITE);
	SDL_RenderClear(eng->renderer);

}

//   ###   HELPERS   ###

static void OptionsMenu_LoadUIStrings(const GameData *data)
{

	OptionsMenuData *optionsMenuData = data->stateData;

	optionsMenuData->uiStrings[OPTIONS_MENU_UI_TITLE]           = "OPTIONS";
	optionsMenuData->uiStrings[OPTIONS_MENU_UI_GEN_NEW_ROSTERS] = "GENERATE NEW ROSTERS";
	optionsMenuData->uiStrings[OPTIONS_MENU_UI_BACK]            = "BACK";
}

static void OptionsMenu_LoadUIData(const GameEngine *eng, const GameData *data)
{
	OptionsMenuData *optionsMenuData = data->stateData;
	
	//Title
	optionsMenuData->uiData[OPTIONS_MENU_UI_TITLE].type = UI_TYPE_TEXT;
	optionsMenuData->uiData[OPTIONS_MENU_UI_TITLE].fg   = COLOR_BLACK;
	
	//New Rosters Button
	UI_SetupDefaultButton(&optionsMenuData->uiData[OPTIONS_MENU_UI_GEN_NEW_ROSTERS]);
	
	//Back Button
	UI_SetupBackButton(&optionsMenuData->uiData[OPTIONS_MENU_UI_BACK]);
	
	//Button onClicks
//	optionsMenuData->uiData[OPTIONS_MENU_UI_GEN_NEW_ROSTERS].onClick = OptionsMenu_GenNewRostersButton_OnClick;
//	optionsMenuData->uiData[OPTIONS_MENU_UI_BACK].onClick = OptionsMenu_BackButton_OnClick;

	OptionsMenu_ResizeLayout(optionsMenuData, data->window.size, data->padding);

	OptionsMenu_CheckButtonHighlight(optionsMenuData->uiData, data->mouse.pos);

	OptionsMenu_CreateTextures(eng, optionsMenuData);
}

static void OptionsMenu_ResizeLayout(OptionsMenuData *data, const Vector2 windowSize, const u8 padding)
{
	(void)padding;
	f32 wX = (f32)windowSize.x;
	f32 wY = (f32)windowSize.y;

	UIData *uiData = NULL;

	//Title
	uiData = &data->uiData[OPTIONS_MENU_UI_TITLE];
	uiData->destRect = UI_GetTitleDestRect(wX, wY);
	
	//General Button area
	
	//Back Button
	

	//REMOVE VOID PADDING AFTER USE

}

static void OptionsMenu_CheckButtonHighlight(UIData *uiData, const FVector2 mousePos)
{
	for (s32 i = OPTIONS_MENU_UI_BUTTON_START; i < OPTIONS_MENU_UI_BUTTON_END; i++) {
		UI_UpdateHover(&uiData[i], mousePos);
	}
}

static void OptionsMenu_CreateTextures(const GameEngine *eng, OptionsMenuData *data)
{
	for (s32 i = OPTIONS_MENU_UI_START; i < OPTIONS_MENU_UI_END; i++) {
		data->uiData[i].texture = Text_CreateUITexture(eng, data->uiStrings[i], &data->uiData[i]);
		if (data->uiData[i].texture == NULL) {
		}
	}
}

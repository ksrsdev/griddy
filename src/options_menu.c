#include "options_menu.h"

#include <stdio.h>
#include <stdlib.h>

#include "colors.h"
#include "context.h"
#include "init.h"
#include "render.h"
#include "state_data.h"
#include "update.h"

static void OptionsMenu_LoadUIStrings(const GameData *data);
static void OptionsMenu_LoadUIData(const GameEngine *eng, const GameData *data);

static void OptionsMenu_ResizeLayout(OptionsMenuData *data, const Vector2 windowSize);
static void OptionsMenu_CreateTextures(const GameEngine *eng, OptionsMenuData *data);

static void OptionsMenu_CheckButtonHighlight(UIData *uiData, const FVector2 mousePos);
static OptionsMenuUIElement OptionsMenu_CheckButtonClick(UIData *uiData, const FVector2 mousePos);

static void OptionsMenu_BackButton_OnClick(GameData *data);

//   ###   INIT   ###
void OptionsMenu_Init(GameEngine *eng, GameData *data) 
{
	data->stateData = calloc(1, sizeof(OptionsMenuData));
	if (data->stateData == nullptr) {
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
			uiData->texture = nullptr;
		}
	}

	Deinit_StateData(&data->stateData);
}

//   ###   UPDATE   ###
void OptionsMenu_Update(GameData *data) 
{
	OptionsMenuData *optionsMenuData = data->stateData;

	if (data->window.resized) {
		OptionsMenu_ResizeLayout(optionsMenuData, data->window.size);
	}
	
	if (data->mouse.moved) {
		OptionsMenu_CheckButtonHighlight(optionsMenuData->uiData, data->mouse.pos);
	}

	if (data->mouse.left.wasPressed) {
		OptionsMenuUIElement clicked = OptionsMenu_CheckButtonClick(optionsMenuData->uiData, data->mouse.pos);

		if (clicked != OPTIONS_MENU_UI_NONE) {
			UIData dataClicked = optionsMenuData->uiData[clicked];
			if (dataClicked.onClick) {
				OnClick onClick = dataClicked.onClick;
				onClick(data);
			}
		}
	}
}

//   ###   RENDER   ###
void OptionsMenu_Render(const GameEngine *eng, const GameData *data) 
{
	OptionsMenuData *optionsMenuData = data->stateData;

	//Clear White
	Render_SetDrawColor(eng->renderer, COLOR_WHITE);
	SDL_RenderClear(eng->renderer);

	//UI Elements
	for (s32 i = OPTIONS_MENU_UI_START; i < OPTIONS_MENU_UI_END; i++) {
		UIData *uiData = &optionsMenuData->uiData[i];
		UI_RenderUIElement(eng, uiData);
	}
}

//   ###   HELPERS   ###

static void OptionsMenu_LoadUIStrings(const GameData *data)
{
	OptionsMenuData *optionsMenuData = data->stateData;

	optionsMenuData->uiStrings[OPTIONS_MENU_UI_TITLE]           = "OPTIONS";
	optionsMenuData->uiStrings[OPTIONS_MENU_UI_GEN_NEW_ROSTERS] = "GENERATE NEW ROSTERS";
	optionsMenuData->uiStrings[OPTIONS_MENU_UI_BACK]            = "<-BACK";
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
	
	//Assign onClick
//	optionsMenuData->uiData[OPTIONS_MENU_UI_GEN_NEW_ROSTERS].onClick = OptionsMenu_GenNewRostersButton_OnClick;
	optionsMenuData->uiData[OPTIONS_MENU_UI_BACK].onClick = OptionsMenu_BackButton_OnClick;

	OptionsMenu_ResizeLayout(optionsMenuData, data->window.size);

	OptionsMenu_CheckButtonHighlight(optionsMenuData->uiData, data->mouse.pos);

	OptionsMenu_CreateTextures(eng, optionsMenuData);
}

static void OptionsMenu_ResizeLayout(OptionsMenuData *data, const Vector2 windowSize)
{
	f32 wX = (f32)windowSize.x;
	f32 wY = (f32)windowSize.y;

	UIData *uiData = nullptr;

	//Title
	uiData = &data->uiData[OPTIONS_MENU_UI_TITLE];
	uiData->dest = UI_GetTitleDestRect(wX, wY);

	//General Buttons
	
	SDL_FRect buttonArea;
	buttonArea.w = wX * 0.50f;
	buttonArea.h = wY * 0.4f;
	buttonArea.x = (wX - buttonArea.w) * 0.5f;
	buttonArea.y = wY * 0.5f;

	s32 numButtons = OPTIONS_MENU_UI_BUTTON_END - OPTIONS_MENU_UI_BUTTON_START;
	s32 numSpaces = numButtons - 1;

	f32 spacesH = (buttonArea.h / 2)  / (f32)numSpaces;

	//Resize Buttons
	for (s32 i = OPTIONS_MENU_UI_BUTTON_START; i < OPTIONS_MENU_UI_BUTTON_END; i++) {
		SDL_FRect *dest = &data->uiData[i].dest;

		dest->w = buttonArea.w;
		dest->h = (buttonArea.h / 2.0f) / (f32)numButtons;
		dest->x = buttonArea.x;
		if (i == OPTIONS_MENU_UI_START) {
			dest->y = buttonArea.y;
		} else {
			dest->y = 
				data->uiData[i - 1].dest.y +
				data->uiData[i - 1].dest.h +
				spacesH;
		}
	}
	
	//Back Button
	uiData = &data->uiData[OPTIONS_MENU_UI_BACK];
	uiData->dest = UI_GetBackButtonDestRect(wX, wY);
	
}

static void OptionsMenu_CreateTextures(const GameEngine *eng, OptionsMenuData *data)
{
	for (s32 i = OPTIONS_MENU_UI_START; i < OPTIONS_MENU_UI_END; i++) {
		data->uiData[i].texture = Text_CreateUITexture(eng, data->uiStrings[i], &data->uiData[i]);
		if (data->uiData[i].texture == nullptr) {
		}
	}
}

static void OptionsMenu_CheckButtonHighlight(UIData *uiData, const FVector2 mousePos)
{
	for (s32 i = OPTIONS_MENU_UI_BUTTON_START; i < OPTIONS_MENU_UI_BUTTON_END; i++) {
		UI_UpdateHover(&uiData[i], mousePos);
	}
}

static OptionsMenuUIElement OptionsMenu_CheckButtonClick(UIData *uiData, const FVector2 mousePos)
{
	for (s32 i = OPTIONS_MENU_UI_BUTTON_START; i < OPTIONS_MENU_UI_BUTTON_END; i++) {
		 if (UI_CheckClick(&uiData[i], mousePos)) {
			 return i;
		 }
	}
	return OPTIONS_MENU_UI_NONE;
}

static void OptionsMenu_BackButton_OnClick(GameData *data)
{
	RequestGameStateTransition(data, data->state.prev);
}

#include "main_menu.h"

#include <stdio.h>
#include <stdlib.h>

#include "colors.h"
#include "context.h"
#include "error.h"
#include "init.h"
#include "render.h"
#include "state_data.h"
#include "types.h"
#include "ui.h"

static void MainMenu_LoadUIStrings(const GameData *data);
static void MainMenu_LoadUIData(const GameEngine *eng, const GameData *data);
static void MainMenu_ResizeLayout(MainMenuData *data, const Vector2 windowSize, const u8 padding);
static void MainMenu_CheckButtonHighlight(UIData *uiDat, const FVector2 mousePos);
static void MainMenu_CreateTextures(const GameEngine *eng, MainMenuData *data);

//   ***   FUNCTION DEFINITIONS   ***

//INIT & DEINIT
void MainMenu_Init(GameEngine *eng, GameData *data)
{
	data->stateData = calloc(1, sizeof(MainMenuData));
	if (data->stateData == NULL) {
		//error.c errors are fatal
		Error_Alert(data, ERROR_ALLOC, "mainMenuData failed calloc()");
		return;
	}

	MainMenu_LoadUIStrings(data);

	MainMenu_LoadUIData(eng, data);
}

void MainMenu_Cleanup(GameEngine *eng, GameData *data)
{
	(void)eng;
	
	MainMenuData *mainMenuData = data->stateData;

	for (s32 i = MAIN_MENU_UI_START; i < MAIN_MENU_UI_END; i++) {

		UIData *uiData = &mainMenuData->uiData[i];
		if (uiData->texture) {
			SDL_DestroyTexture(uiData->texture);
			uiData->texture = NULL;
		}
	}

	Deinit_StateData(&data->stateData);
}

//   ###   UPDATE   ###

void MainMenu_Update(GameData *data)
{
	MainMenuData *mainMenuData = data->stateData;

	if (data->window.resized) {
		MainMenu_ResizeLayout(mainMenuData, data->window.size, data->padding);
	}

}

//   ###   RENDER   ###

void MainMenu_Render(const GameEngine *eng, const GameData *data)
{
	MainMenuData *mainMenuData = data->stateData;

	//Render BG
	Render_SetDrawColor(eng->renderer, COLOR_WHITE);
	SDL_RenderClear(eng->renderer);
	
	//Render UI Elements
	for (s32 i = MAIN_MENU_UI_START; i < MAIN_MENU_UI_END; i++) {
		UIData *uiData = &mainMenuData->uiData[i];
		SDL_SetRenderDrawColor(eng->renderer, 255, 0, 255, 255);
		SDL_RenderRect(eng->renderer,  &uiData->destRect);
		UI_RenderUIElement(eng, uiData);
	}

}

//   ###   HELPERS   ###

static void MainMenu_LoadUIStrings(const GameData *data)
{
	MainMenuData *mainMenuData = data->stateData;

	mainMenuData->uiStrings[MAIN_MENU_UI_TITLE]   = "GRIDDY";
	mainMenuData->uiStrings[MAIN_MENU_UI_SPLASH]  = "SHAKE IT!";
	mainMenuData->uiStrings[MAIN_MENU_UI_PLAY]    = "PLAY GAME";
	mainMenuData->uiStrings[MAIN_MENU_UI_OPTIONS] = "OPTIONS";
	mainMenuData->uiStrings[MAIN_MENU_UI_EXIT]    = "EXIT";
	mainMenuData->uiStrings[MAIN_MENU_UI_VERSION] = "SDL_Test";
}

static void MainMenu_LoadUIData(const GameEngine *eng, const GameData *data)
{
	(void)eng;
	MainMenuData *mainMenuData = data->stateData;

	mainMenuData->uiData[MAIN_MENU_UI_TITLE].type = UI_TYPE_TEXT;
	mainMenuData->uiData[MAIN_MENU_UI_TITLE].fg   = COLOR_BLACK;

	mainMenuData->uiData[MAIN_MENU_UI_SPLASH].type     = UI_TYPE_TEXT;
	mainMenuData->uiData[MAIN_MENU_UI_SPLASH].fg       = COLOR_RED;
	mainMenuData->uiData[MAIN_MENU_UI_SPLASH].rotation = -30.0;
	
	mainMenuData->uiData[MAIN_MENU_UI_VERSION].type = UI_TYPE_TEXT;
	mainMenuData->uiData[MAIN_MENU_UI_VERSION].fg   = COLOR_GREY;

	//Buttons
	for (s32 i = MAIN_MENU_UI_BUTTON_START; i < MAIN_MENU_UI_BUTTON_END; i++) {
		UI_SetupDefaultButton(&mainMenuData->uiData[i]);
	}

	MainMenu_ResizeLayout(mainMenuData, data->window.size, data->padding);

	MainMenu_CheckButtonHighlight(mainMenuData->uiData, data->mouse.pos);

	MainMenu_CreateTextures(eng, mainMenuData);

	//Call it twice because we need the actual texture sizes to align the splash correctly
	MainMenu_ResizeLayout(mainMenuData, data->window.size, data->padding);
}

static void MainMenu_ResizeLayout(MainMenuData *data, const Vector2 windowSize, const u8 padding)
{
	f32 wX = (f32)windowSize.x;
	f32 wY = (f32)windowSize.y;

	//General layout in words:
	//General 10% top and bottom margin
	//Title 20% tall and 33% wide
	//Splash should pulse centered on the bottom right corner of title and angled yeah
	//Buttons occupy 40% of the screen with 10% padding from title and 20% from bottom
	//version can go in a small corner, maybe 5% by 5% from bottom left corner

	//Title
	data->uiData[MAIN_MENU_UI_TITLE].destRect.w = wX * 0.50f;
	data->uiData[MAIN_MENU_UI_TITLE].destRect.h = wY * 0.20f;
	data->uiData[MAIN_MENU_UI_TITLE].destRect.y = wY * 0.10f;
	data->uiData[MAIN_MENU_UI_TITLE].destRect.x = (wX - data->uiData[MAIN_MENU_UI_TITLE].destRect.w) * 0.5f;
	
	//Splash
	data->uiData[MAIN_MENU_UI_SPLASH].destRect.w = 
		data->uiData[MAIN_MENU_UI_TITLE].destRect.w / 3;

	data->uiData[MAIN_MENU_UI_SPLASH].destRect.h = 
		data->uiData[MAIN_MENU_UI_TITLE].destRect.h / 2;

	s32 titleTextureW;
	if (data->uiData[MAIN_MENU_UI_TITLE].texture) {
		titleTextureW = data->uiData[MAIN_MENU_UI_TITLE].texture->w;
	} else {
		titleTextureW = 42;
	}
	f32 titleDestW    = data->uiData[MAIN_MENU_UI_TITLE].destRect.w;

	data->uiData[MAIN_MENU_UI_SPLASH].destRect.x =  
		data->uiData[MAIN_MENU_UI_TITLE].destRect.x +
		data->uiData[MAIN_MENU_UI_TITLE].destRect.w -
		(data->uiData[MAIN_MENU_UI_SPLASH].destRect.w / 2) -
		((titleDestW - (f32)titleTextureW) * 0.5f) - padding;


	s32 titleTextureH;
	if (data->uiData[MAIN_MENU_UI_TITLE].texture) {
		titleTextureH = data->uiData[MAIN_MENU_UI_TITLE].texture->h;
	} else {
		titleTextureH = 42;
	}
	f32 titleDestH    = data->uiData[MAIN_MENU_UI_TITLE].destRect.h;

	data->uiData[MAIN_MENU_UI_SPLASH].destRect.y =  
		data->uiData[MAIN_MENU_UI_TITLE].destRect.y +
		data->uiData[MAIN_MENU_UI_TITLE].destRect.h -
		((titleDestH - (f32)titleTextureH) / 2) - padding -
		(data->uiData[MAIN_MENU_UI_TITLE].destRect.h / 6);
	
	//Version
	data->uiData[MAIN_MENU_UI_VERSION].destRect.w = wX * 0.10f;
	data->uiData[MAIN_MENU_UI_VERSION].destRect.h = wY * 0.10f;
	data->uiData[MAIN_MENU_UI_VERSION].destRect.y = wY - data->uiData[MAIN_MENU_UI_VERSION].destRect.h - 25.0f;
	data->uiData[MAIN_MENU_UI_VERSION].destRect.x = 25.0f;

	//Buttons - Define button area
	SDL_FRect buttonArea;
	buttonArea.w = wX * 0.50f;
	buttonArea.h = wY * 0.4f;
	buttonArea.x = (wX - buttonArea.w) * 0.5f;
	buttonArea.y = wY * 0.4f;

	//num buttons and spaces
	s32 numButtons = MAIN_MENU_UI_BUTTON_END - MAIN_MENU_UI_BUTTON_START;
	s32 numSpaces = numButtons - 1;

	//Total space is 30% = total buttons is 70%
	f32 spacesH = (buttonArea.h / 4)  / (f32)numSpaces;
	//f32 buttonsH = (buttonArea.h / 7) / (f32)numButtons;

	//Resize Buttons
	for (s32 i = MAIN_MENU_UI_BUTTON_START; i < MAIN_MENU_UI_BUTTON_END; i++) {
		data->uiData[i].destRect.w = buttonArea.w;
		data->uiData[i].destRect.h = 32;
		data->uiData[i].destRect.x = buttonArea.x;
		if (i == MAIN_MENU_UI_BUTTON_START) {
			data->uiData[i].destRect.y = buttonArea.y;
		} else {
		data->uiData[i].destRect.y = 
			data->uiData[i - 1].destRect.y + 
			data->uiData[i - 1].destRect.h + 
			spacesH;
		}
	}
}

static void MainMenu_CheckButtonHighlight(UIData *uiData, const FVector2 mousePos)
{
	for (s32 i = MAIN_MENU_UI_BUTTON_START; i < MAIN_MENU_UI_BUTTON_END; i++) {
		UI_UpdateHover(&uiData[i], mousePos);
	}
}

static void MainMenu_CreateTextures(const GameEngine *eng, MainMenuData *data)
{
	for (s32 i = MAIN_MENU_UI_START; i < MAIN_MENU_UI_END; i++) {
		data->uiData[i].texture = Text_CreateUITexture(eng, data->uiStrings[i], &data->uiData[i]);
		if (data->uiData[i].texture == NULL) {
		}
	}
}



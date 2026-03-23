#include "main_menu.h"

#include <stdlib.h>

#include "colors.h"
#include "context.h"
#include "error.h"
#include "init.h"
#include "state_data.h"
#include "types.h"
#include "ui.h"

static void MainMenu_LoadUIStrings(const GameData *data);
static void MainMenu_LoadUIData(const GameEngine *eng, const GameData *data);
static void MainMenu_ResizeLayout(MainMenuData *data, const Vector2 windowSize);
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

	Deinit_StateData(&data->stateData);

}

//UPDATE

void MainMenu_Update(GameData *data)
{
	(void)data;
}

//RENDER

void MainMenu_Render(const GameEngine *eng, const GameData *data)
{
	(void)eng;
	(void)data;
}


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

	MainMenu_ResizeLayout(mainMenuData, data->window.size);

	MainMenu_CheckButtonHighlight(mainMenuData->uiData, data->mouse.pos);

	MainMenu_CreateTextures(eng, mainMenuData);
}

static void MainMenu_ResizeLayout(MainMenuData *data, const Vector2 windowSize)
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
	data->uiData[MAIN_MENU_UI_TITLE].destRect.w = wX * 0.33f;
	data->uiData[MAIN_MENU_UI_TITLE].destRect.h = wY * 0.20f;
	data->uiData[MAIN_MENU_UI_TITLE].destRect.y = wY * 0.10f;
	data->uiData[MAIN_MENU_UI_TITLE].destRect.x = (wX - data->uiData[MAIN_MENU_UI_TITLE].destRect.w) * 0.5f;
	
	//Splash
	
	//Version
	
	//Play
	
	//Options
	
	//

}

static void MainMenu_CheckButtonHighlight(UIData *uiData, const FVector2 mousePos)
{
	for (s32 i = MAIN_MENU_UI_BUTTON_START; i < MAIN_MENU_UI_BUTTON_END; i++) {
		UI_UpdateHover(&uiData[i], mousePos);
	}
}

static void MainMenu_CreateTextures(const GameEngine *eng, MainMenuData *data)
{
	for (s32 i = MAIN_MENU_UI_BUTTON_START; i < MAIN_MENU_UI_BUTTON_END; i++) {
		data->uiData[i].texture = Text_CreateUITexture(eng, data->uiStrings[i], &data->uiData[i]);
	}
}



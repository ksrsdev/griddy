#include "main_menu.h"

#include <stdio.h>
#include <stdlib.h>

#include "colors.h"
#include "context.h"
#include "error.h"
#include "init.h"
#include "render.h"
#include "splash_text.h"
#include "state_data.h"
#include "team.h"
#include "types.h"
#include "ui.h"
#include "util.h"
#include "update.h"

static void MainMenu_LoadUIStrings(const GameData *data);
static void MainMenu_LoadUIData(const GameEngine *eng, const GameData *data);

static void MainMenu_ResizeLayout(MainMenuData *data, const Vector2 windowSize, const u8 padding);
static SDL_FRect MainMenu_GetSplashDestRect(MainMenuData *data, const u8 padding);
static void  MainMenu_ResizeSplash(MainMenuData *data, const u8 padding);
static void MainMenu_CheckButtonHighlight(UIData *uiDat, const FVector2 mousePos);
static void MainMenu_CreateTextures(const GameEngine *eng, MainMenuData *data);

static void MainMenu_CheckButtonHighlight(UIData *uiData, const FVector2 mousePos);
static MainMenuUIElement  MainMenu_CheckButtonClick(UIData *uiData, const FVector2 mousePos);

static void MainMenu_PlayButton_OnClick(GameData *data);
static void MainMenu_OptionsButton_OnClick(GameData *data);
static void MainMenu_ExitButton_OnClick(GameData *data);

static void ClearSelectedTeams(GameData *data);

//   ***   FUNCTION DEFINITIONS   ***

//INIT & DEINIT
void MainMenu_Init(GameEngine *eng, GameData *data)
{
	//Clear selected teams (this is the place to do that...i think)
	ClearSelectedTeams(data);

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

	//Update splash pulse anim timer
	mainMenuData->pulseCurrTime = SDL_GetTicks();

	if (data->window.resized) {
		MainMenu_ResizeLayout(mainMenuData, data->window.size, data->padding);
	}

	//Resize splash every frame for pulse anim
	MainMenu_ResizeSplash(mainMenuData, data->padding);
	
	if (data->mouse.moved) {
		MainMenu_CheckButtonHighlight(mainMenuData->uiData, data->mouse.pos);
	}
	
	if (data->mouse.left.wasPressed) {
		s32 elementClicked = MainMenu_CheckButtonClick(mainMenuData->uiData, data->mouse.pos);
		
		if (elementClicked != MAIN_MENU_UI_NONE) {
			UIData dataClicked = mainMenuData->uiData[elementClicked];
			if (dataClicked.onClick) {
				OnClick onClick = dataClicked.onClick;
				onClick(data);
			}
		}
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
		UI_RenderUIElement(eng, uiData);
	}
}

//   ###   HELPERS   ###

static void MainMenu_LoadUIStrings(const GameData *data)
{
	MainMenuData *mainMenuData = data->stateData;

	mainMenuData->uiStrings[MAIN_MENU_UI_TITLE]   = "GRIDDY";
	mainMenuData->uiStrings[MAIN_MENU_UI_PLAY]    = "PLAY GAME";
	mainMenuData->uiStrings[MAIN_MENU_UI_OPTIONS] = "OPTIONS";
	mainMenuData->uiStrings[MAIN_MENU_UI_EXIT]    = "EXIT";
	mainMenuData->uiStrings[MAIN_MENU_UI_VERSION] = "SDL_Test";

	//splash text
	const char *splashText = GetSplashText();
	if (splashText == NULL) {
		splashText = "ERROR";
	}

	mainMenuData->uiStrings[MAIN_MENU_UI_SPLASH] = splashText;
}

static void MainMenu_LoadUIData(const GameEngine *eng, const GameData *data)
{
	MainMenuData *mainMenuData = data->stateData;
	

	//Title
	mainMenuData->uiData[MAIN_MENU_UI_TITLE].type = UI_TYPE_TEXT;
	mainMenuData->uiData[MAIN_MENU_UI_TITLE].fg   = COLOR_BLACK;

	//Init Splash Vars
	mainMenuData->pulseBaseTime = SDL_GetTicks();
	mainMenuData->pulseCurrTime = mainMenuData->pulseBaseTime + 1;

	//Splash
	mainMenuData->uiData[MAIN_MENU_UI_SPLASH].type     = UI_TYPE_TEXT;
	mainMenuData->uiData[MAIN_MENU_UI_SPLASH].fg       = COLOR_RED;
	mainMenuData->uiData[MAIN_MENU_UI_SPLASH].rotation = -30.0;

	//Version
	mainMenuData->uiData[MAIN_MENU_UI_VERSION].type = UI_TYPE_TEXT;
	mainMenuData->uiData[MAIN_MENU_UI_VERSION].fg   = COLOR_GREY;

	//Buttons
	for (s32 i = MAIN_MENU_UI_BUTTON_START; i < MAIN_MENU_UI_BUTTON_END; i++) {
		UI_SetupDefaultButton(&mainMenuData->uiData[i]);
	}

	//Button onClicks
	mainMenuData->uiData[MAIN_MENU_UI_EXIT].onClick = MainMenu_ExitButton_OnClick;
	mainMenuData->uiData[MAIN_MENU_UI_OPTIONS].onClick = MainMenu_OptionsButton_OnClick;
	mainMenuData->uiData[MAIN_MENU_UI_PLAY].onClick = MainMenu_PlayButton_OnClick;


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
	
	UIData *title = &data->uiData[MAIN_MENU_UI_TITLE];
	title->destRect = UI_GetTitleDestRect(wX, wY);

	//Splash
	data->uiData[MAIN_MENU_UI_SPLASH].destRect = MainMenu_GetSplashDestRect(data, padding);
	
	//Version
	data->uiData[MAIN_MENU_UI_VERSION].destRect.w = wX * 0.10f;
	data->uiData[MAIN_MENU_UI_VERSION].destRect.h = wY * 0.10f;
	data->uiData[MAIN_MENU_UI_VERSION].destRect.x = 25.0f;

	//Get Y value
	data->uiData[MAIN_MENU_UI_VERSION].destRect.y = wY - data->uiData[MAIN_MENU_UI_VERSION].destRect.h - 25.0f;

	f32 versionYOffest = MIN(data->uiData[MAIN_MENU_UI_VERSION].destRect.h, 64 + 25);
	versionYOffest = wY - versionYOffest;
	
	data->uiData[MAIN_MENU_UI_VERSION].destRect.y = versionYOffest;


	//Buttons - Define button area
	SDL_FRect buttonArea;
	buttonArea.w = wX * 0.50f;
	buttonArea.h = wY * 0.4f;
	buttonArea.x = (wX - buttonArea.w) * 0.5f;
	buttonArea.y = wY * 0.5f;

	//num buttons and spaces
	s32 numButtons = MAIN_MENU_UI_BUTTON_END - MAIN_MENU_UI_BUTTON_START;
	s32 numSpaces = numButtons - 1;

	//Total space is 30% = total buttons is 70%
	f32 spacesH = (buttonArea.h / 2)  / (f32)numSpaces;
	//f32 buttonsH = (buttonArea.h / 7) / (f32)numButtons;

	//Resize Buttons
	for (s32 i = MAIN_MENU_UI_BUTTON_START; i < MAIN_MENU_UI_BUTTON_END; i++) {
		data->uiData[i].destRect.w = buttonArea.w;
		data->uiData[i].destRect.h = (buttonArea.h / 2) / (f32)numButtons;
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

static SDL_FRect MainMenu_GetSplashDestRect(MainMenuData *data, const u8 padding)
{
	UIData titleData = data->uiData[MAIN_MENU_UI_TITLE];

	SDL_FRect splashDest = {0, 0, 0, 0};
	SDL_FRect titleDest = titleData.destRect;

	f32 titleTextureW;
	f32 titleTextureH;
	if (titleData.texture) {
		titleTextureW = (f32)titleData.texture->w;
		titleTextureH = (f32)titleData.texture->h;
	} else {
		titleTextureW = 32;
		titleTextureH = 32;
	}

	//apply padding ie convert titleDest to titlepaddedDest
	titleDest.x += padding;
	titleDest.y += padding;
	titleDest.w = titleDest.w - (padding * 2);
	titleDest.h = titleDest.h - (padding * 2);

	f32 scaleX = 1;
	f32 scaleY = 1;
	
	if (titleData.texture) {
		scaleX = (titleDest.w - (padding * 2)) / titleTextureW;
		scaleY = (titleDest.h - (padding * 2)) / titleTextureH;
	}

	f32 finalScale = MIN(scaleX, scaleY);

	SDL_FRect titleTightRect;
	titleTightRect.w = titleTextureW * finalScale;
	titleTightRect.h = titleTextureH * finalScale;
	titleTightRect.x = titleDest.x + ((titleDest.w - titleTightRect.w) / 2);
	titleTightRect.y = titleDest.y + ((titleDest.h - titleTightRect.h) / 2);


	if (data->pulseCurrTime < data->pulseBaseTime) {
		//ERROR
		SDL_Log("pulseCurrTime < pulseBaseTime!");
		printf("pulseCurrTime < pulseBaseTime!\n");
		SDL_FRect wrongRect = {0, 0, 0, 0};
		return wrongRect;
	}

	f32 pulseMod = 0;
	f32 pulseScale = 2;
	u64 deltaTime = data->pulseCurrTime - data->pulseBaseTime;

	if (deltaTime > 2000) {
		data->pulseBaseTime = data->pulseCurrTime;
		pulseMod = 0;
	} else if (deltaTime > 1000) {
		pulseMod = 1000.0f - ((f32)deltaTime - 1000.0f);
	} else {
		pulseMod = (f32)deltaTime;
	}

	pulseScale = (1.0f / 3.0f) + (pulseMod / 6000.0f);

	//W
	splashDest.w = titleTightRect.w * pulseScale;

	//H
	splashDest.h = titleTightRect.h * pulseScale;

	//X
	splashDest.x = titleTightRect.x + titleTightRect.w - (splashDest.w / 2);

	//Y
	splashDest.y = titleTightRect.y + titleTightRect.h - (splashDest.h / 2);

	return splashDest;
}

static void  MainMenu_ResizeSplash(MainMenuData *data, const u8 padding)
{
	data->uiData[MAIN_MENU_UI_SPLASH].destRect = MainMenu_GetSplashDestRect(data, padding);
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

static MainMenuUIElement  MainMenu_CheckButtonClick(UIData *uiData, const FVector2 mousePos)
{
	for (s32 i = MAIN_MENU_UI_BUTTON_START; i < MAIN_MENU_UI_BUTTON_END; i++) {
		 if (UI_CheckClick(&uiData[i], mousePos)) {
			 return i;
		 }
	}
	return MAIN_MENU_UI_NONE;
}

static void MainMenu_ExitButton_OnClick(GameData *data)
{
	data->isRunning = false;
}

static void MainMenu_OptionsButton_OnClick(GameData *data)
{
	RequestGameStateTransition(data, GAME_STATE_OPTIONS_MENU);
}

static void MainMenu_PlayButton_OnClick(GameData *data)
{
	RequestGameStateTransition(data, GAME_STATE_TEAM_SELECT);
}


static void ClearSelectedTeams(GameData *data)
{

	data->playerTeamId = TEAM_ID_NONE;
	data->cpuTeamId = TEAM_ID_NONE;
	data->previewTeamId = TEAM_ID_NONE;

}


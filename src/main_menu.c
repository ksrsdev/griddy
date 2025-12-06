#include "button.h"
#include "main.h"
#include "main_menu.h"
#include "raylib.h"

void MainMenuDrawButtons(void);
bool buttonsMainMenuReady = false;

Button MainMenuButtons[MAIN_MENU_BUTTON_COUNT];


void InitMainMenuButtons() 
{
	MainMenuButtons[0] = MakeButton("RELOAD", BLACK);
	MainMenuButtons[1] = MakeButton("EXIT", BLACK);
}

void MainMenuDrawButtons(void)
{
	if (IsWindowResized()) {
		buttonsMainMenuReady = false;
	}
	if (!buttonsMainMenuReady) {
		ResizeButtonArray(MainMenuButtons, MAIN_MENU_BUTTON_COUNT, 10);
	}
}

void DrawMainMenu(void)
{
	//Clear
	ClearBackground(RAYWHITE);
	//Draw Griddy - should be shaped via screen size
	DrawText("GRIDDY", 25, 25, 25, BLACK);
	//Draw fun message
	//Draw Buttons - center 33% of the screen
	MainMenuDrawButtons();
	//Check Button Press
}

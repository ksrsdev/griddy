#include "button.h"
#include "main.h"
#include "main_menu.h"
#include "raylib.h"

void MainMenuDrawButtons(void);
void DrawMainMenuTitle(void);


bool buttonsMainMenuReady = false;

Button MainMenuButtons[MAIN_MENU_BUTTON_COUNT];

void DrawMainMenuTitle(void)
{
	//MarginX same as buttons - 33
	//Margin Y should be the middle of the top margin
	//top margin is 33
	//33 / 2 ~= 16.5
	//so it should be height about 16.5 of the screen
	//with a y margin of 8.25, yes?
	char *titleText = "GRIDDY!";
	float screenWidth = (float)GetScreenWidth();
	float screenHeight = (float)GetScreenHeight();
	float marginX = (33 / 100.0f) * screenWidth;
	float marginY = (8.25f / 100.0f) * screenHeight;
	float textPosX, textPosY;
	int fontSize;
	Vector2 textSize, textBox;
	textBox.x = screenWidth - (2 * marginX);
	textBox.y = marginY * 4;
	textPosX = marginX;
	textPosY = marginY;
	fontSize = 1;
	textSize = MeasureTextEx(GetFontDefault(), titleText, (float)fontSize, 1.0f);
	while (textSize.x < textBox.x && textSize.y < textBox.y) {
		fontSize++;
		textSize = MeasureTextEx(GetFontDefault(), titleText, (float)fontSize, 1.0f);
	}
	DrawText(titleText, (int)textPosX, (int)textPosY, fontSize, BLACK);
}

void InitMainMenuButtons() 
{
	MainMenuButtons[0] = MakeButton("QUICK GAME", GRAY);
	MainMenuButtons[1] = MakeButton("NEW GAME", GRAY);
	MainMenuButtons[2] = MakeButton("LOAD GAME", GRAY);
	MainMenuButtons[3] = MakeButton("RELOAD", GRAY);
	MainMenuButtons[4] = MakeButton("EXIT", GRAY);
}

void MainMenuDrawButtons(void)
{
	if (IsWindowResized()) {
		buttonsMainMenuReady = false;
	}
	if (!buttonsMainMenuReady) {
		//Resize et Reposition
		RepositionButtonArray_CenteredVertical(MainMenuButtons, MAIN_MENU_BUTTON_COUNT, 33, 33);
		//Draw
		DrawButtonArray(MainMenuButtons, MAIN_MENU_BUTTON_COUNT);
	}
}

void DrawMainMenu(void)
{
	//Clear
	ClearBackground(RAYWHITE);
	//Draw Griddy - should be shaped via screen size
	DrawMainMenuTitle();
	//Draw fun message
	//Draw Buttons - center 33% of the screen
	MainMenuDrawButtons();
	//Check Button Press
}

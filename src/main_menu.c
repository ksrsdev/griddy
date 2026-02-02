#include "button.h"
#include "global.h"
#include "init.h"
#include "startup.h"
#include "main_menu.h"
#include "raylib.h"
#include "team_select.h"
#include "text.h"

//functions
void MainMenuDrawButtons(void);
void DrawMainMenuTitle(void);
void DrawMainMenuVersionNumber(void);
void MainMenuCheckButtonPress(void);
//variables
bool buttonsMainMenuReady = false;
//button array
Button MainMenuButtons[MAIN_MENU_BUTTON_COUNT];

//definitions
void DrawMainMenuTitle(void)
{
	//MarginX same as buttons - 33
	//Margin Y should be the middle of the top margin
	//top margin is 33
	//33 / 2 ~= 16.5
	//so it should be height about 16.5 of the screen
	//with a y margin of 8.25, yes?
	char *titleText = "GRIDDY!";
	DrawMenuTitleText(titleText);
}


void DrawMainMenuVersionNumber(void)
{
	char *versionText = "ALPHA A.1";
	float screenWidth = (float)GetScreenWidth();
	float screenHeight = (float)GetScreenHeight();
	float marginX = screenWidth / 20.0f;
	float marginY = screenHeight / 20.0f;
	Vector2 textSize, textPos;
	textPos.x = marginX;
	textPos.y = screenHeight - marginY;
	int fontSize = 1;
	textSize = MeasureTextEx(GetFontDefault(), versionText, (float)fontSize, 1.0f);
	while (textSize.x < screenWidth / 20.0f && textSize.y < screenHeight / 20.0f) {
		fontSize++;
		textSize = MeasureTextEx(GetFontDefault(), versionText, (float)fontSize, 1.0f);
	}
	DrawTextEx(GetFontDefault(), versionText, textPos, (float)fontSize, 1.0f, BLACK);
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
		RepositionButtonArray_CenteredVertical(MainMenuButtons, MAIN_MENU_BUTTON_COUNT, 17, 33);
		//Draw
		DrawButtonArray(MainMenuButtons, MAIN_MENU_BUTTON_COUNT);
	}
}

void MainMenuCheckButtonPress(void)
{
	int press = CheckButtonArrayForButtonPress(MainMenuButtons, MAIN_MENU_BUTTON_COUNT);
	if (press == -1) {
		return;
	}
	switch (press) {
		case 0:
			InitTeamSelect();
			griddy.state = MAIN_GAME_STATE_QUICK_GAME_PLAYER_TEAM_SELECT;
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			InitStartup();
			griddy.state = MAIN_GAME_STATE_STARTUP;
			break;
		case 4:
			griddy.gameRunning = false;
			break;
		default:
			TraceLog(LOG_INFO, "press OOB");
			break;
	}

}

void DrawMainMenu(void)
{
	//Clear
	ClearBackground(RAYWHITE);
	//Draw Griddy and splash - should be shaped via screen size
	DrawMainMenuTitle();
	//Draw version number
	DrawMainMenuVersionNumber();
	//Draw Buttons - center 33% of the screen
	MainMenuDrawButtons();
	//Check Button Press
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		MainMenuCheckButtonPress(); 
	}
}

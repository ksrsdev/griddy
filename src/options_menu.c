#include "button.h"
#include "global.h"
#include "options_menu.h"
#include "player.h"
#include "text.h"

#include "raylib.h"

void DrawOptionsMenu_Title(void);
void DrawOptionsMenu_Buttons(void);

void OptionsMenuCheckButtonPress(void);

#define OPTIONS_MENU_BUTTON_COUNT 3
Button OptionsMenuButtons[OPTIONS_MENU_BUTTON_COUNT];
Button optionsMenuBackButton;
bool optionsMenuButtonsReady = false;

void InitOptionsMenuButtons() 
{
	//Back Button
	optionsMenuBackButton = MakeButton("<- BACK", RED);
	//Options List
	OptionsMenuButtons[0] = MakeButton("GENERATE PLAYER FILE", GRAY);
	OptionsMenuButtons[1] = MakeButton("READ GENERATED PLAYER FILE", GRAY);
	OptionsMenuButtons[2] = MakeButton("GENERATE ALL ROSTER FILES", GRAY);
}

void DrawOptionsMenu_Title(void)
{
	char *optionsText = "OPTIONS";
	DrawMenuTitleText(optionsText);
}

void DrawOptionsMenu_Buttons(void)
{
	if (IsWindowResized()) {
		optionsMenuButtonsReady = false;
	}
	if (!optionsMenuButtonsReady) {
		RepositionButtonArray_CenteredVertical(OptionsMenuButtons, OPTIONS_MENU_BUTTON_COUNT, 17, 33);
		RepositionSingleButton_BottomLeft(&optionsMenuBackButton);
		optionsMenuButtonsReady = true;
	}
	DrawSingleButton(&optionsMenuBackButton);
	DrawButtonArray(OptionsMenuButtons, OPTIONS_MENU_BUTTON_COUNT);
}

void OptionsMenuCheckButtonPress(void)
{
	//back button
	if (CheckSingleButtonForButtonPress(&optionsMenuBackButton)) {
		griddy.state = MAIN_GAME_STATE_MAIN_MENU;
		return;
	}
	int press = CheckButtonArrayForButtonPress(OptionsMenuButtons, OPTIONS_MENU_BUTTON_COUNT);
	if (press == -1) {
		return;
	}
	switch (press) {
		case 0:
			GenRandomPlayer();
			break;
		case 1:
			TestLoadPlayerFromFile();
			return;
		case 2:
			GenerateAllRosters();
			return;
		default:
			TraceLog(LOG_INFO, "press OOB");
			break;
	}
}

void DrawOptionsMenu(void)
{
	//clear
	ClearBackground(RAYWHITE);
	//Options Title
	DrawOptionsMenu_Title();
	//Draw Buttons
	DrawOptionsMenu_Buttons();
	//Check Button Press
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		OptionsMenuCheckButtonPress(); 
	}
}

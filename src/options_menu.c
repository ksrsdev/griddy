
#include "button.h"
#include "options_menu.h"
#include "raylib.h"
#include "text.h"

void DrawOptionsMenu_Title(void);
void DrawOptionsMenu_Buttons(void);


#define OPTIONS_MENU_BUTTON_COUNT 2
Button OptionsMenuButtons[OPTIONS_MENU_BUTTON_COUNT];
bool optionsMenuButtonsReady = false;

void InitOptionsMenuButtons() 
{
	OptionsMenuButtons[0] = MakeButton("GENERATE PLAYER FILE", GRAY);
	OptionsMenuButtons[1] = MakeButton("RETURN", GRAY);
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
		DrawButtonArray(OptionsMenuButtons, OPTIONS_MENU_BUTTON_COUNT);
		optionsMenuButtonsReady = true;
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


}

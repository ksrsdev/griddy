#include "button.h"
#include "main.h"
#include "raylib.h"
#include "team_select.h"

void TeamSelectDrawTitleText(void);
void TeamSelectDrawButtons(void);
void TeamSelectCheckButtonPress(void);

bool buttonsTeamSelectReady = false;
Button teamSelectBackButton;
Button TeamSelectButtons[TEAM_SELECT_BUTTONS_COUNT];

void InitTeamSelectButtons(void)
{
	//Back Button
	teamSelectBackButton = MakeButton("<- BACK", RED);
	//Team Select Buttons
	TeamSelectButtons[0] = MakeButton("BLACK", BLACK);
	TeamSelectButtons[1] = MakeButton("WHITE", WHITE);
}

void TeamSelectDrawTitleText(void)
{
	char *titleText = "Select Team";
	float screenWidth = (float)GetScreenWidth();
	float screenHeight = (float)GetScreenHeight();
	float marginX = screenWidth * (33.0f / 100.0f);
	float marginY = (8.25f / 100.0f) * screenHeight;
	Vector2 textSize, textBox, textPos;
	textBox.x = screenWidth - (2 * marginX);
	textBox.y = marginY * 4;
	textPos.x = marginX;
	textPos.y = marginY;
	int fontSize = 1;
	textSize = MeasureTextEx(GetFontDefault(), titleText, (float)fontSize, 1.0f);
	while (textSize.x < textBox.x && textSize.y < textBox.y) {
		fontSize++;
		textSize = MeasureTextEx(GetFontDefault(), titleText, (float)fontSize, 1.0f);
	}
	DrawTextEx(GetFontDefault(), titleText, textPos, (float)fontSize, 1.0f, BLACK);
}

void TeamSelectDrawButtons(void)
{
	if (IsWindowResized()) {
		buttonsTeamSelectReady = false;
	}
	if (!buttonsTeamSelectReady) {
		//Resize et Reposition
		//back button
		RepositionSingleButton_BottomLeft(&teamSelectBackButton);
		DrawSingleButton(&teamSelectBackButton);
		//team select buttons
		//RepositionButtonArray_CenteredHorizontal(TeamSelectButtons)
		//DrawButtonArray(TeamSelectButtons)
	}
}

void TeamSelectCheckButtonPress(void)
{
	//back button
	if (CheckSingleButtonForButtonPress(&teamSelectBackButton)) {
		mainGameState = MAIN_GAME_STATE_MAIN_MENU;
	}
	//team select buttons

}

void DrawQuickGameTeamSelect(void)
{
	//Clear
	ClearBackground(RAYWHITE);
	//Select Text
	TeamSelectDrawTitleText();
	//Text Box
	//draw buttons (back, team buttons)
	TeamSelectDrawButtons();
	//back button
	//TeamSelectDrawBackButton();
	//team buttons
	//check button press
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		TeamSelectCheckButtonPress();
	}
}

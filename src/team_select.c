#include "button.h"
#include "main.h"
#include "raylib.h"
#include "team_select.h"

void TeamSelectDrawTitleText(void);
void TeamSelectDrawButtons(void);
void TeamSelect_DrawBackButton(void);
void TeamSelect_DrawTeamButtons(void);
void TeamSelectCheckButtonPress(void);
void RepositionButtonArray_TeamSelectButtons(void);

bool buttonsTeamSelectReady = false;
Button teamSelectBackButton;
Button TeamSelectButtons_Row1[TEAM_SELECT_BUTTONS_ROW1_COUNT];
Button TeamSelectButtons_Row2[TEAM_SELECT_BUTTONS_ROW2_COUNT];

void InitTeamSelectButtons(void)
{
	//Back Button
	teamSelectBackButton = MakeButton("<- BACK", RED);
	//Team Select Buttons Row 1
	TeamSelectButtons_Row1[0] = MakeButton("BLACK", BLACK);
	TeamSelectButtons_Row1[1] = MakeButton("WHITE", WHITE);
	TeamSelectButtons_Row1[2] = MakeButton("GREEN", GREEN);
	TeamSelectButtons_Row1[3] = MakeButton("RED", RED);
	//Team Select But_Row1tons Row 2
	TeamSelectButtons_Row2[0] = MakeButton("PINK", PINK);
	TeamSelectButtons_Row2[1] = MakeButton("BROWN", BROWN);
	TeamSelectButtons_Row2[2] = MakeButton("YELLOW", YELLOW);
	TeamSelectButtons_Row2[3] = MakeButton("ORANGE", ORANGE);
	TeamSelectButtons_Row2[4] = MakeButton("BLUE", BLUE);

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

void RepositionButtonArray_TeamSelectButtons(void)
{
	//What this has to do is determine the rowY value for RepositionButtonArray_VariableHorizontal
	//Given as a percentage of course
	RepositionButtonArray_VariableHorizontal(TeamSelectButtons_Row1, TEAM_SELECT_BUTTONS_ROW1_COUNT, 30, 10, 33);
	RepositionButtonArray_VariableHorizontal(TeamSelectButtons_Row2, TEAM_SELECT_BUTTONS_ROW2_COUNT, 30, 10, 50);
}

void TeamSelect_DrawBackButton(void) 
{
	RepositionSingleButton_BottomLeft(&teamSelectBackButton);
	DrawSingleButton(&teamSelectBackButton);
}

void TeamSelect_DrawTeamButtons(void)
{
	RepositionButtonArray_TeamSelectButtons();
	DrawButtonArray(TeamSelectButtons_Row1, TEAM_SELECT_BUTTONS_ROW1_COUNT);
	DrawButtonArray(TeamSelectButtons_Row2, TEAM_SELECT_BUTTONS_ROW2_COUNT);
}

void TeamSelectDrawButtons(void)
{
	if (IsWindowResized()) {
		buttonsTeamSelectReady = false;
	}
	if (!buttonsTeamSelectReady) { //resize
		//back button
		TeamSelect_DrawBackButton();
		//team select buttons
		TeamSelect_DrawTeamButtons();
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

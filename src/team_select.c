#include "button.h"
#include "main.h"
#include "raylib.h"
#include "team_select.h"

void InitTeamSelectButtons(void);

void TeamSelectDrawTitleText(void);
void TeamSelectDrawButtons(void);
void TeamSelect_DrawBackButton(void);
void TeamSelect_DrawContinueButton(void);
void TeamSelect_DrawTeamButtons(void);
void TeamSelectCheckButtonPress(void);
void RepositionButtonArray_TeamSelectButtons(void);
void TeamSelect_UpdateRandomColorHue(void);

float teamSelectRandomButtonHue = 0.0f;

bool buttonsTeamSelectReady = false;
Button teamSelectBackButton;
Button teamSelectContinueButton;
Button TeamSelectButtons_Row1[TEAM_SELECT_BUTTONS_ROW1_COUNT];
Button TeamSelectButtons_Row2[TEAM_SELECT_BUTTONS_ROW2_COUNT];

int playerTeamSelected;

void InitTeamSelect(void)
{
	playerTeamSelected = 0;
	InitTeamSelectButtons();
}

void InitTeamSelectButtons(void)
{
	//Back Button
	teamSelectBackButton = MakeButton("<- BACK", RED);
	//Random
	teamSelectContinueButton = MakeButton("CONTINUE ->", GRAY);
	//Team Select Buttons Row 1
	TeamSelectButtons_Row1[0] = MakeButton("RANDOM", RED);
	TeamSelectButtons_Row1[1] = MakeButton("BLACK", BLACK);
	TeamSelectButtons_Row1[2] = MakeButton("WHITE", WHITE);
	TeamSelectButtons_Row1[3] = MakeButton("GREEN", GREEN);
	TeamSelectButtons_Row1[4] = MakeButton("RED", RED);
	//Team Select But_Row1tons Row 2
	TeamSelectButtons_Row2[0] = MakeButton("PINK", PINK);
	TeamSelectButtons_Row2[1] = MakeButton("BROWN", BROWN);
	TeamSelectButtons_Row2[2] = MakeButton("YELLOW", YELLOW);
	TeamSelectButtons_Row2[3] = MakeButton("ORANGE", ORANGE);
	TeamSelectButtons_Row2[4] = MakeButton("BLUE", BLUE);

}

void TeamSelectDrawTitleText(void)
{
	char *titleText = "Select Your Team";
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

void TeamSelect_DrawContinueButton(void) 
{
	if (playerTeamSelected == 0) {
		teamSelectContinueButton.visible = false;
	} else {
		teamSelectContinueButton.visible = true;
	}
	RepositionSingleButton_BottomRight(&teamSelectContinueButton);
	DrawSingleButton(&teamSelectContinueButton);
}

void TeamSelect_UpdateRandomColorHue(void)
{
	TraceLog(LOG_INFO, "Update Color");
	if (teamSelectRandomButtonHue > 360.0f) {
		teamSelectRandomButtonHue = 0.0f;
	} else {
		teamSelectRandomButtonHue++;
	}
	Color rainbowColor = ColorFromHSV(teamSelectRandomButtonHue, 1.0f, 1.0f);
	TeamSelectButtons_Row1[0].bg_color = rainbowColor;
}

void TeamSelect_DrawTeamButtons(void)
{
	//update Random Color Hue
	TeamSelect_UpdateRandomColorHue();
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
		//continue buttton
		TeamSelect_DrawContinueButton();
	}
	//draw team select buttons every frame to accomodate the ??? color hue changer
	TeamSelect_DrawTeamButtons();
}

void TeamSelectCheckButtonPress(void)
{
	//back button
	if (CheckSingleButtonForButtonPress(&teamSelectBackButton)) {
		mainGameState = MAIN_GAME_STATE_MAIN_MENU;
	}
	//continue button
	if (CheckSingleButtonForButtonPress(&teamSelectContinueButton) && playerTeamSelected > 0) {
		mainGameState = MAIN_GAME_STATE_STARTUP; //NOTE: NOT FINISHED
	}
	//team select buttons
	int press = CheckButtonArrayForButtonPress(TeamSelectButtons_Row1, TEAM_SELECT_BUTTONS_ROW1_COUNT);
	if (press != -1) {
		switch (press) {
			//Random
			case 0:
				playerTeamSelected = 1;
				break;
			//Black
			case 1:
				playerTeamSelected = 2;
				break;
			//White
			case 2:
				playerTeamSelected = 3;
				break;
			//Green
			case 3:
				playerTeamSelected = 4;
				break;
			//Red
			case 4:
				playerTeamSelected = 5;
				break;
		}
	}
	press = CheckButtonArrayForButtonPress(TeamSelectButtons_Row2, TEAM_SELECT_BUTTONS_ROW2_COUNT);
	if (press == -1) {
		return;
	}
	switch (press) {
		//Pink
		case 0:
			playerTeamSelected = 6;
			break;
		//Brown
		case 1:
			playerTeamSelected = 7;
			break;
		//Yellow
		case 2:
			playerTeamSelected = 8;
			break;
		//Orange
		case 3:
			playerTeamSelected = 9;
			break;
		//Blue
		case 4:
			playerTeamSelected = 10;
			break;
	}
}

void DrawQuickGameTeamSelect(void)
{
	//Clear
	ClearBackground(RAYWHITE);
	//Select Your Team Title Text
	TeamSelectDrawTitleText();
	//draw buttons (back, team buttons, random button)
	TeamSelectDrawButtons();
	//draw text box
	//check button press
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		TeamSelectCheckButtonPress();
	}
}

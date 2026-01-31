#include "button.h"
#include "init.h"
#include "main.h"
#include "raylib.h"
#include "team_select.h"
#include "text.h"

void InitTeamSelectButtons(void);

void TeamSelectDrawTitleText(void);
void TeamSelectDrawButtons(void);
void TeamSelect_DrawBackButton(void);
void TeamSelect_DrawContinueButton(void);
void TeamSelect_DrawTeamButtons(void);
void TeamSelect_HidePlayerChosenTeamButton(void);
void TeamSelectDrawTextBox(void);
void TeamSelectDrawTextBoxDescriptionText(Rectangle textBoxRec, int currentTeamSelection);
void TeamSelectCheckButtonPress(void);
void RepositionButtonArray_TeamSelectButtons(void);
void TeamSelect_UpdateRandomColorHue(void);

float teamSelectRandomButtonHue = 0.0f;

bool buttonsTeamSelectReady = false;
Button teamSelectBackButton;
Button teamSelectContinueButton;
Button TeamSelectButtons_Row1[TEAM_SELECT_BUTTONS_ROW1_COUNT];
Button TeamSelectButtons_Row2[TEAM_SELECT_BUTTONS_ROW2_COUNT];

int playerTeamSelected = TEAM_SELECTED_NONE;
int cpuTeamSelected = TEAM_SELECTED_NONE;

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
	char *titleText = "INIT";
	if (mainGameState == MAIN_GAME_STATE_QUICK_GAME_PLAYER_TEAM_SELECT) {
		titleText = "Select Your Team";
	} else {
		titleText = "Select CPU Team";
	}
	DrawMenuTitleText(titleText);
	//float screenWidth = (float)GetScreenWidth();
	//float screenHeight = (float)GetScreenHeight();
	//float marginX = screenWidth * (33.0f / 100.0f);
	//float marginY = (8.25f / 100.0f) * screenHeight;
	//Vector2 textSize, textBox, textPos;
	//textBox.x = screenWidth - (2 * marginX);
	//textBox.y = marginY * 4;
	//textPos.x = marginX;
	//textPos.y = marginY;
	//int fontSize = 1;
	//textSize = MeasureTextEx(GetFontDefault(), titleText, (float)fontSize, 1.0f);
	//while (textSize.x < textBox.x && textSize.y < textBox.y) {
	//	fontSize++;
	//	textSize = MeasureTextEx(GetFontDefault(), titleText, (float)fontSize, 1.0f);
	//}
	//DrawTextEx(GetFontDefault(), titleText, textPos, (float)fontSize, 1.0f, BLACK);
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
	int currentTeamSelection;
	//Player Team Select
	if (mainGameState == MAIN_GAME_STATE_QUICK_GAME_PLAYER_TEAM_SELECT) {
		currentTeamSelection = playerTeamSelected;
	} else {
		currentTeamSelection = cpuTeamSelected;
	}
	if (currentTeamSelection == TEAM_SELECTED_NONE) {
		teamSelectContinueButton.visible = false;
	} else {
		teamSelectContinueButton.visible = true;
	}
	RepositionSingleButton_BottomRight(&teamSelectContinueButton);
	DrawSingleButton(&teamSelectContinueButton);
}

void TeamSelect_UpdateRandomColorHue(void)
{
	if (teamSelectRandomButtonHue > 360.0f) {
		teamSelectRandomButtonHue = 0.0f;
	} else {
		teamSelectRandomButtonHue++;
	}
	Color rainbowColor = ColorFromHSV(teamSelectRandomButtonHue, 1.0f, 1.0f);
	TeamSelectButtons_Row1[0].bg_color = rainbowColor;
}

void TeamSelect_HidePlayerChosenTeamButton(void)
{
	switch (playerTeamSelected) {
		case TEAM_SELECTED_NONE:
		case TEAM_SELECTED_RANDOM:
			break;
		case TEAM_SELECTED_BLACK:
			TeamSelectButtons_Row1[1].visible = false;
			break;
		case TEAM_SELECTED_WHITE:
			TeamSelectButtons_Row1[2].visible = false;
			break;
		case TEAM_SELECTED_GREEN:
			TeamSelectButtons_Row1[3].visible = false;
			break;
		case TEAM_SELECTED_RED:
			TeamSelectButtons_Row1[4].visible = false;
			break;
		case TEAM_SELECTED_PINK:
			TeamSelectButtons_Row2[0].visible = false;
			break;
		case TEAM_SELECTED_BROWN:
			TeamSelectButtons_Row2[1].visible = false;
			break;
		case TEAM_SELECTED_YELLOW:
			TeamSelectButtons_Row2[2].visible = false;
			break;
		case TEAM_SELECTED_ORANGE:
			TeamSelectButtons_Row2[3].visible = false;
			break;
		case TEAM_SELECTED_BLUE:
			TeamSelectButtons_Row2[4].visible = false;
			break;
	}
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

void TeamSelectDrawTextBox(void)
{
	Rectangle textBoxRec;
	float screenWidth = (float)GetScreenWidth();
	float screenHeight = (float)GetScreenHeight();
	 textBoxRec.x = screenWidth / 3.0f;
	 textBoxRec.y = screenHeight * 2.0f / 3.0f;
	 textBoxRec.width = screenWidth / 3.0f;
	 textBoxRec.height = screenHeight * 0.15f;
	//Draw the empty box - Unless team color is white or yellow then we need a black box
	int currentTeamSelection;
	if (mainGameState == MAIN_GAME_STATE_QUICK_GAME_PLAYER_TEAM_SELECT) {
		currentTeamSelection = playerTeamSelected;
	} else {
		currentTeamSelection = cpuTeamSelected;
	}
	if (currentTeamSelection == TEAM_SELECTED_WHITE || currentTeamSelection == TEAM_SELECTED_YELLOW) {
		DrawRectangleRec(textBoxRec, BLACK);
	} else {
		DrawRectangleLinesEx(textBoxRec, 2.0, BLACK);
	}
	//Draw the description text
	TeamSelectDrawTextBoxDescriptionText(textBoxRec, currentTeamSelection);

}

void TeamSelectDrawTextBoxDescriptionText(Rectangle textBoxRec, int currentTeamSelection) 
{
	char *descText = "INIT";
	Color textColor;
	switch (currentTeamSelection) {
		case TEAM_SELECTED_NONE:
			if (mainGameState == MAIN_GAME_STATE_QUICK_GAME_PLAYER_TEAM_SELECT) {
				descText = "Select a team to play as!";
			} else {
				descText = "Select a team for the CPU!";
			}
			textColor = BLACK;
			break;
		case TEAM_SELECTED_RANDOM:
			descText = "Let fate decide!";
			textColor = ColorFromHSV(teamSelectRandomButtonHue, 1.0f, 1.0f);
			break;
		case TEAM_SELECTED_BLACK:
			descText = "Black team selected!";
			textColor = BLACK;
			break;
		case TEAM_SELECTED_WHITE:
			descText = "White team selected!";
			textColor = WHITE;
			break;
		case TEAM_SELECTED_GREEN:
			descText = "Green team selected!";
			textColor = GREEN;
			break;
		case TEAM_SELECTED_RED:
			descText = "Red team selected!";
			textColor = RED;
			break;
		case TEAM_SELECTED_PINK:
			descText = "Pink team selected!";
			textColor = PINK;
			break;
		case TEAM_SELECTED_BROWN:
			descText = "Brown team selected!";
			textColor = BROWN;
			break;
		case TEAM_SELECTED_YELLOW:
			descText = "Yellow team selected!";
			textColor = YELLOW;
			break;
		case TEAM_SELECTED_ORANGE:
			descText = "Orange team selected!";
			textColor = ORANGE;
			break;
		case TEAM_SELECTED_BLUE:
			descText = "Blue team selected!";
			textColor = BLUE;
			break;
	}
	Vector2 textSize, textBoxSize, textPos;
	textBoxSize.x = textBoxRec.width - (textBoxRec.width / 10); 
	textBoxSize.y = textBoxRec.height;
	textPos.x = textBoxRec.x;
	textPos.y = textBoxRec.y;
	int fontSize = 1;
	textSize = MeasureTextEx(GetFontDefault(), descText, (float)fontSize, 1.0f);
	while (textSize.x < (textBoxSize.x - (textBoxSize.x / 5)) && textSize.y < textBoxSize.y) {
		fontSize++;
		textSize = MeasureTextEx(GetFontDefault(), descText, (float)fontSize, 1.0f);
	}
	textPos.x += (textBoxSize.x - textSize.x) / 2;
	textPos.y += (textBoxSize.y - textSize.y) / 2;
	DrawTextEx(GetFontDefault(), descText, textPos, (float)fontSize, 1.0f, textColor);

}

void TeamSelectCheckButtonPress(void)
{
	//back button
	if (CheckSingleButtonForButtonPress(&teamSelectBackButton)) {
		if (mainGameState == MAIN_GAME_STATE_QUICK_GAME_PLAYER_TEAM_SELECT) {
			mainGameState = MAIN_GAME_STATE_MAIN_MENU;
		} else {
			InitTeamSelect();
			mainGameState = MAIN_GAME_STATE_QUICK_GAME_PLAYER_TEAM_SELECT;
		}
	}
	//continue button
	//Player Select
	if (mainGameState == MAIN_GAME_STATE_QUICK_GAME_PLAYER_TEAM_SELECT) {
		if (CheckSingleButtonForButtonPress(&teamSelectContinueButton) && playerTeamSelected > 0) {
			TeamSelect_HidePlayerChosenTeamButton();
			mainGameState = MAIN_GAME_STATE_QUICK_GAME_CPU_TEAM_SELECT; 
		}
	//CPU Select
	} else {
		if (CheckSingleButtonForButtonPress(&teamSelectContinueButton) && cpuTeamSelected > 0) {
			InitQuickGameConfirm();
			mainGameState = MAIN_GAME_STATE_QUICK_GAME_CONFIRM; //place holder, should init game
		}
	}
	//team select buttons
	int press = CheckButtonArrayForButtonPress(TeamSelectButtons_Row1, TEAM_SELECT_BUTTONS_ROW1_COUNT);
	int currentTeamSelection = TEAM_SELECTED_NONE;
	if (press != -1) {
		switch (press) {
			//Random
			case 0:
				currentTeamSelection = TEAM_SELECTED_RANDOM;
				break;
			//Black
			case 1:
				currentTeamSelection = TEAM_SELECTED_BLACK;
				break;
			//White
			case 2:
				currentTeamSelection = TEAM_SELECTED_WHITE;
				break;
			//Green
			case 3:
				currentTeamSelection = TEAM_SELECTED_GREEN;
				break;
			//Red
			case 4:
				currentTeamSelection = TEAM_SELECTED_RED;
				break;
		}
	}
	press = CheckButtonArrayForButtonPress(TeamSelectButtons_Row2, TEAM_SELECT_BUTTONS_ROW2_COUNT);
	if ( mainGameState == MAIN_GAME_STATE_QUICK_GAME_PLAYER_TEAM_SELECT) {
		playerTeamSelected = currentTeamSelection;
	} else {
		cpuTeamSelected = currentTeamSelection;
	}
	if (press == -1) {
		return;
	}
	switch (press) {
		//Pink
		case 0:
			currentTeamSelection = TEAM_SELECTED_PINK;
			break;
		//Brown
		case 1:
			currentTeamSelection = TEAM_SELECTED_BROWN;
			break;
		//Yellow
		case 2:
			currentTeamSelection = TEAM_SELECTED_YELLOW;
			break;
		//Orange
		case 3:
			currentTeamSelection = TEAM_SELECTED_ORANGE;
			break;
		//Blue
		case 4:
			currentTeamSelection = TEAM_SELECTED_BLUE;
			break;
	}
	if ( mainGameState == MAIN_GAME_STATE_QUICK_GAME_PLAYER_TEAM_SELECT) {
		playerTeamSelected = currentTeamSelection;
	} else {
		cpuTeamSelected = currentTeamSelection;
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
	TeamSelectDrawTextBox();
	//check button press
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		TeamSelectCheckButtonPress();
	}
}

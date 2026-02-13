#include "button.h"
#include "global.h"
#include "init.h"
#include "raylib.h"
#include "team.h"
#include "team_select.h"
#include "text.h"
#include "util.h"

#include <stdlib.h>
#include <time.h>

void TeamSelectDrawTitleText(void);
void TeamSelectDrawButtons(void);
void TeamSelect_DrawBackButton(void);
void TeamSelect_DrawContinueButton(void);
void TeamSelect_DrawTeamButtons(void);
void TeamSelect_HidePlayerChosenTeamButton(void);
void TeamSelectDrawTextBox(void);
void TeamSelectDrawTextBoxDescriptionText(Rectangle *textBoxRec, int currentTeamSelection);
void TeamSelectCheckButtonPress(void);
void RepositionButtonArray_TeamSelectButtons(void);
void TeamSelect_UpdateRandomColorHue(void);

static float teamSelectRandomButtonHue = 0.0f;

bool buttonsTeamSelectReady = false;
Button teamSelectBackButton;
Button teamSelectContinueButton;
Button TeamSelectButtons_Row1[TEAM_SELECT_BUTTONS_ROW1_COUNT];
Button TeamSelectButtons_Row2[TEAM_SELECT_BUTTONS_ROW2_COUNT];

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

void ResolveRandomTeam(void)
{
	//Note on this
	//rand() is INCLUSIVE, TEAM_COUNT is the end and RANDOM comes right after TEAM_NONE
	TeamId randomTeam = (rand() % ((TEAM_COUNT - 1) - (TEAM_RANDOM + 1) + 1)) + (TEAM_RANDOM + 1);
	if (griddy.playerTeam == TEAM_RANDOM) {
		while (randomTeam == griddy.cpuTeam) {
			randomTeam = (rand() % ((TEAM_COUNT - 1) - (TEAM_RANDOM + 1) + 1)) + (TEAM_RANDOM + 1);
		}
		griddy.playerTeam = randomTeam;
	}
	if (griddy.cpuTeam == TEAM_RANDOM) {
		while (randomTeam == griddy.playerTeam) {
			randomTeam = (rand() % ((TEAM_COUNT - 1) - (TEAM_RANDOM + 1) + 1)) + (TEAM_RANDOM + 1);
		}
		griddy.cpuTeam = randomTeam;
	}
}

void TeamSelectDrawTitleText(void)
{
	char *titleText = "INIT";
	if (griddy.state == MAIN_GAME_STATE_QUICK_GAME_PLAYER_TEAM_SELECT) {
		titleText = "Select Your Team";
	} else {
		titleText = "Select CPU Team";
	}
	DrawMenuTitleText(titleText);
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
	if (griddy.state == MAIN_GAME_STATE_QUICK_GAME_PLAYER_TEAM_SELECT) {
		currentTeamSelection = griddy.playerTeam;
	} else {
		currentTeamSelection = griddy.cpuTeam;
	}
	if (currentTeamSelection == TEAM_NONE) {
		teamSelectContinueButton.visible = false;
	} else {
		teamSelectContinueButton.visible = true;
	}
	RepositionSingleButton_BottomRight(&teamSelectContinueButton);
	DrawSingleButton(&teamSelectContinueButton);
}

void TeamSelect_UpdateRandomColorHue(void)
{
	teamSelectRandomButtonHue = CycleHue(teamSelectRandomButtonHue);
	Color rainbowColor = ColorFromHSV(teamSelectRandomButtonHue, 1.0f, 1.0f);
	TeamSelectButtons_Row1[0].bg_color = rainbowColor;
}

void TeamSelect_HidePlayerChosenTeamButton(void)
{
	switch (griddy.playerTeam) {
		case TEAM_NONE:
		case TEAM_COUNT:
		case TEAM_RANDOM:
			break;
		case TEAM_BLACK:
			TeamSelectButtons_Row1[1].visible = false;
			break;
		case TEAM_WHITE:
			TeamSelectButtons_Row1[2].visible = false;
			break;
		case TEAM_GREEN:
			TeamSelectButtons_Row1[3].visible = false;
			break;
		case TEAM_RED:
			TeamSelectButtons_Row1[4].visible = false;
			break;
		case TEAM_PINK:
			TeamSelectButtons_Row2[0].visible = false;
			break;
		case TEAM_BROWN:
			TeamSelectButtons_Row2[1].visible = false;
			break;
		case TEAM_YELLOW:
			TeamSelectButtons_Row2[2].visible = false;
			break;
		case TEAM_ORANGE:
			TeamSelectButtons_Row2[3].visible = false;
			break;
		case TEAM_BLUE:
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
	 textBoxRec.x = (2.0f * (screenWidth / 33.0f)) + (screenWidth / 15.0f);
	 textBoxRec.y = screenHeight * 0.6f;
	 textBoxRec.width = screenWidth - (2.0f * textBoxRec.x);
	 textBoxRec.height = screenHeight - (screenHeight * 2.0f / 3.0f) - (2.0f * (screenWidth / 50.0f));
	//Draw the empty box - Unless team color is white or yellow then we need a black box
	int currentTeamSelection;
	if (griddy.state == MAIN_GAME_STATE_QUICK_GAME_PLAYER_TEAM_SELECT) {
		currentTeamSelection = griddy.playerTeam;
	} else {
		currentTeamSelection = griddy.cpuTeam;
	}
	if (currentTeamSelection == TEAM_WHITE || currentTeamSelection == TEAM_YELLOW) {
		DrawRectangleRec(textBoxRec, BLACK);
	} else {
		DrawRectangleLinesEx(textBoxRec, 2.0, BLACK);
	}
	//Draw the description text
	TeamSelectDrawTextBoxDescriptionText(&textBoxRec, currentTeamSelection);
}

void TeamSelectDrawTextBoxDescriptionText(Rectangle *textBoxRec, int currentTeamSelection) 
{
	const TeamData *teamData = GetTeamData(currentTeamSelection);
	Color teamColor = teamData->color;
	Rectangle targetTextBox = {0};
	targetTextBox.width = textBoxRec->width * 0.9f;
	targetTextBox.x = textBoxRec->x + ((textBoxRec->width - targetTextBox.width) /2);
	targetTextBox.y = textBoxRec->y;
	targetTextBox.height = textBoxRec->height;
	//handle team none and team random
	if (currentTeamSelection == TEAM_RANDOM) {
		teamColor = ColorFromHSV(teamSelectRandomButtonHue, 1.0f, 1.0f);
		DrawTextInBoxColor(teamData->blueprint->desc, &targetTextBox, &teamColor);
		return;
	}
	if (currentTeamSelection == TEAM_NONE) {
		DrawTextInBoxColor(teamData->blueprint->desc, &targetTextBox, &teamColor);
		return;
	}
	//Team Selected - first 25%
	targetTextBox.width = textBoxRec->width - (textBoxRec->width / 10.0f);
	targetTextBox.x = textBoxRec->x + ((textBoxRec->width - targetTextBox.width) / 2);
	targetTextBox.y = textBoxRec->y;
	targetTextBox.height = textBoxRec->height / 4.0f;
	DrawTextInBoxColor(teamData->blueprint->name, &targetTextBox, &teamColor);
	//Desc - second 25%
	targetTextBox.y += targetTextBox.height;
	DrawTextInBoxColor(teamData->blueprint->desc, &targetTextBox, &teamColor);
	//pros - 50/3
	targetTextBox.y += targetTextBox.height;
	targetTextBox.height = textBoxRec->height / 6.0f;
	char targetText[64];
	snprintf(targetText, sizeof(targetText), "Strengths: %s", teamData->blueprint->pros);
	DrawTextInBoxColor(targetText, &targetTextBox, &teamColor);
	//cons 50/3
	targetTextBox.y += targetTextBox.height;
	snprintf(targetText, sizeof(targetText), "Weaknesses: %s", teamData->blueprint->cons);
	DrawTextInBoxColor(targetText, &targetTextBox, &teamColor);
	//offense and defense forms (note these are two sub boxes centered on left and right thirds of screen
	//offense first - left
	targetTextBox.y += targetTextBox.height;
	targetTextBox.y -= (targetTextBox.height / 10.0f); 
	targetTextBox.width = targetTextBox.width / 2.0f;
	snprintf(targetText, sizeof(targetText), "Offense: %s", teamData->blueprint->off);
	DrawTextInBoxColor(targetText, &targetTextBox, &teamColor);
	//defense - right
	targetTextBox.x += targetTextBox.width;
	snprintf(targetText, sizeof(targetText), "Defense: %s", teamData->blueprint->def);
	DrawTextInBoxColor(targetText, &targetTextBox, &teamColor);
	



	//Down here is / was old method



//	Vector2 textSize, textBoxSize, textPos;
//	textBoxSize.x = textBoxRec.width - (textBoxRec.width / 10); 
//	textBoxSize.y = textBoxRec.height;
//	textPos.x = textBoxRec.x;
//	textPos.y = textBoxRec.y;
//	int fontSize = 1;
//	textSize = MeasureTextEx(GetFontDefault(), descText, (float)fontSize, 1.0f);
//	while (textSize.x < (textBoxSize.x - (textBoxSize.x / 5)) && textSize.y < textBoxSize.y) {
//		fontSize++;
//		textSize = MeasureTextEx(GetFontDefault(), descText, (float)fontSize, 1.0f);
//	}
//	textPos.x += (textBoxSize.x - textSize.x) / 2;
//	textPos.y += (textBoxSize.y - textSize.y) / 2;
//	DrawTextEx(GetFontDefault(), descText, textPos, (float)fontSize, 1.0f, textColor);
}

void TeamSelectCheckButtonPress(void)
{
	//back button
	if (CheckSingleButtonForButtonPress(&teamSelectBackButton)) {
		if (griddy.state == MAIN_GAME_STATE_QUICK_GAME_PLAYER_TEAM_SELECT) {
			griddy.state = MAIN_GAME_STATE_MAIN_MENU;
			return;
		} else {
			InitTeamSelect();
			griddy.state = MAIN_GAME_STATE_QUICK_GAME_PLAYER_TEAM_SELECT;
			return;
		}
	}
	//continue button
	//Player Select
	if (griddy.state == MAIN_GAME_STATE_QUICK_GAME_PLAYER_TEAM_SELECT) {
		if (CheckSingleButtonForButtonPress(&teamSelectContinueButton) && griddy.playerTeam > 0) {
			TeamSelect_HidePlayerChosenTeamButton();
			griddy.state = MAIN_GAME_STATE_QUICK_GAME_CPU_TEAM_SELECT; 
			return;
		}
	//CPU Select
	} else {
		if (CheckSingleButtonForButtonPress(&teamSelectContinueButton) && griddy.cpuTeam > 0) {
			InitQuickGameConfirm();
			griddy.state = MAIN_GAME_STATE_QUICK_GAME_CONFIRM; 
			return;
		}
	}
	//team select buttons
	int press = CheckButtonArrayForButtonPress(TeamSelectButtons_Row1, TEAM_SELECT_BUTTONS_ROW1_COUNT);
	int currentTeamSelection = TEAM_NONE;
	if (press != -1) {
		switch (press) {
			//Random
			case 0:
				currentTeamSelection = TEAM_RANDOM;
				break;
			//Black
			case 1:
				currentTeamSelection = TEAM_BLACK;
				break;
			//White
			case 2:
				currentTeamSelection = TEAM_WHITE;
				break;
			//Green
			case 3:
				currentTeamSelection = TEAM_GREEN;
				break;
			//Red
			case 4:
				currentTeamSelection = TEAM_RED;
				break;
		}
	}
	press = CheckButtonArrayForButtonPress(TeamSelectButtons_Row2, TEAM_SELECT_BUTTONS_ROW2_COUNT);
	if ( griddy.state == MAIN_GAME_STATE_QUICK_GAME_PLAYER_TEAM_SELECT) {
		griddy.playerTeam = currentTeamSelection;
	} else {
		griddy.cpuTeam = currentTeamSelection;
	}
	if (press == -1) {
		return;
	}
	switch (press) {
		//Pink
		case 0:
			currentTeamSelection = TEAM_PINK;
			break;
		//Brown
		case 1:
			currentTeamSelection = TEAM_BROWN;
			break;
		//Yellow
		case 2:
			currentTeamSelection = TEAM_YELLOW;
			break;
		//Orange
		case 3:
			currentTeamSelection = TEAM_ORANGE;
			break;
		//Blue
		case 4:
			currentTeamSelection = TEAM_BLUE;
			break;
	}
	if (griddy.state == MAIN_GAME_STATE_QUICK_GAME_PLAYER_TEAM_SELECT) {
		griddy.playerTeam = currentTeamSelection;
	} else {
		griddy.cpuTeam = currentTeamSelection;
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

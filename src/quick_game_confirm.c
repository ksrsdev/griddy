#include "button.h"
#include "global.h"
#include "init.h"
#include "quick_game_confirm.h"
#include "raylib.h"
#include "team.h"
#include "team_select.h"
#include "text.h"
#include "util.h"

#include <stdio.h>

void QuickGameConfirm_DrawBackButton(void);
void QuickGameConfirm_CheckButtonPress(void);
void QuickGameConfirm_DrawInfoBoxes(void);

Rectangle GetLeftInfoBoxDimensions(const float screenWidth, float screenHeight);
Rectangle GetRightInfoBoxDimensions(const float screenWidth, float screenHeight);

void PopulateTeamSummaryInfoBox(const TeamData *teamData, const Rectangle *infoBox);

Button quickGameConfirmBackButton;

float randomColorHue = 0.0f;

void InitQuickGameConfirmButtons(void)
{
	quickGameConfirmBackButton = MakeButton("<- BACK", RED);
}

void QuickGameConfirm_DrawBackButton(void)
{
	RepositionSingleButton_BottomLeft(&quickGameConfirmBackButton);
	DrawSingleButton(&quickGameConfirmBackButton);
}

Rectangle GetLeftInfoBoxDimensions(const float screenWidth, const float screenHeight)
{
	Rectangle infoBox;
	infoBox.x = screenWidth / 10.0f;
	infoBox.y = screenHeight / 5.0f;
	infoBox.width = screenWidth / 3.0f;
	infoBox.height = screenHeight * (0.6f);
	return infoBox;
}

Rectangle GetRightInfoBoxDimensions(const float screenWidth, const float screenHeight)
{
	Rectangle infoBox;
	infoBox.y = screenHeight / 5.0f;
	infoBox.width = screenWidth / 3.0f;
	infoBox.x = screenWidth - (screenWidth / 10.0f) - infoBox.width;
	infoBox.height = screenHeight * (0.6f);
	return infoBox;
}

void PopulateTeamSummaryInfoBox(const TeamData *teamData, const Rectangle *infoBox)
{
	//Just draw the different sections one at a time top to bottom
	//Create target textBox - this is the box we will pass to the Draw functions
	Rectangle targetTextBox;
	//X and width are equal and don't change (until players and button :/)
	targetTextBox.width = infoBox->width - (infoBox->width / 10);
	targetTextBox.x = infoBox->x + ((infoBox-> width - targetTextBox.width) / 2);
	//height is determined by the section (title 20, name 20, players 50/3, roster 10)
	targetTextBox.y = infoBox->y;
	targetTextBox.height = infoBox->height / 5;
	
	//Title - top 20%
	if (teamData->id == griddy.playerTeam) {
		DrawTextInBox("Player Team", &targetTextBox);
	} else {
		DrawTextInBox("CPU Team", &targetTextBox);
	}
	//Seperator line
	DrawLine((int)infoBox->x, (int)targetTextBox.y + (int)targetTextBox.height, (int)infoBox->x + (int)infoBox->width, (int)targetTextBox.y + (int)targetTextBox.height, BLACK);

	//Team Name - 20%
	const char *teamName = teamData->name;
	char teamText[32];
	snprintf(teamText, sizeof(teamText), "%s team", teamName);
	Color teamColor = teamData->color;
	targetTextBox.y += targetTextBox.height;

	DrawTextInBoxColor(teamText, &targetTextBox, &teamColor);	

	//The entire player section should be 50 %
	//top player 1 50/3%
	//top player 2 50/3%
	//top player 3 50/3%
	//
	//Roster button - 10%
	
}

void QuickGameConfirm_DrawInfoBoxes(void)
{
	//Assign info box dimensions
	float screenWidth = (float)GetScreenWidth();
	float screenHeight = (float)GetScreenHeight();
	Rectangle playerInfoBox, cpuInfoBox;
	playerInfoBox = GetLeftInfoBoxDimensions(screenWidth, screenHeight);
	cpuInfoBox = GetRightInfoBoxDimensions(screenWidth, screenHeight);
	//Draw Outlines
	DrawRectangleLinesEx(playerInfoBox, 2.0, BLACK);
	DrawRectangleLinesEx(cpuInfoBox, 2.0, BLACK);
	//Asign Team Data
	const TeamData *playerTeamData = GetTeamData(griddy.playerTeam);
	const TeamData *cpuTeamData = GetTeamData(griddy.cpuTeam);
	//Do the actual text rendering
	PopulateTeamSummaryInfoBox(playerTeamData, &playerInfoBox); 
	PopulateTeamSummaryInfoBox(cpuTeamData, &cpuInfoBox); 
	//Populate with text



	//Below should be put into the Populate() function


	//Depth Chart Button

}

void QuickGameConfirm_CheckButtonPress(void)
{
	//Back button
	if (CheckSingleButtonForButtonPress(&quickGameConfirmBackButton)) {
		InitTeamSelect();
		griddy.state = MAIN_GAME_STATE_QUICK_GAME_PLAYER_TEAM_SELECT;
	}
}

void DrawQuickGameConfirm(void)
{
	//Clear
	ClearBackground(RAYWHITE);
	//Title
	DrawMenuTitleText("Quick Game Confirm");
	//Info Boxes
	QuickGameConfirm_DrawInfoBoxes();
	//Player info box
	//CPU info box
	//home team graphic
	//weather option
	//start button
	//back button
	QuickGameConfirm_DrawBackButton();
	//check button press
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		QuickGameConfirm_CheckButtonPress();
	}
}

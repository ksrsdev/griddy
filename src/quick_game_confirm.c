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

	//Team Name
	const char *teamName = teamData->name;
	char teamText[32];
	snprintf(teamText, sizeof(teamText), "%s team", teamName);
	Color teamColor = teamData->color;
	Rectangle teamNameTextBox;
	teamNameTextBox.width = infoBox->width / 3.3f;
	teamNameTextBox.x = infoBox->x + teamNameTextBox.width;
	teamNameTextBox.height = infoBox->height / 5.0f;
	teamNameTextBox.y = infoBox->y + (infoBox->height / 12.0f);
	
	DrawTextInBoxColor(teamText, &teamNameTextBox, &teamColor);	
	//pros:
	char teamPros[32];

	snprintf(teamPros, sizeof(teamPros), "Pros: %s", teamData->pros);
	//cons:
	
	//top player 1
	//top player 2
	//top player 3
	
}

void QuickGameConfirm_DrawInfoBoxes(void)
{
	//Outlines First
	float screenWidth = (float)GetScreenWidth();
	float screenHeight = (float)GetScreenHeight();
	Rectangle playerInfoBox, cpuInfoBox;
	playerInfoBox = GetLeftInfoBoxDimensions(screenWidth, screenHeight);
	cpuInfoBox = GetRightInfoBoxDimensions(screenWidth, screenHeight);

	//cpu info box assingment
	DrawRectangleLinesEx(playerInfoBox, 2.0, BLACK);
	DrawRectangleLinesEx(cpuInfoBox, 2.0, BLACK);

	//Populate with text

	//Title
	DrawInfoBoxTitleText("Player Team", &playerInfoBox);
	DrawInfoBoxTitleText("CPU Team", &cpuInfoBox);

	//Team Name
	const TeamData *playerTeamData = GetTeamData(griddy.playerTeam);
	const TeamData *cpuTeamData = GetTeamData(griddy.cpuTeam);
	PopulateTeamSummaryInfoBox(playerTeamData, &playerInfoBox); 
	PopulateTeamSummaryInfoBox(cpuTeamData, &cpuInfoBox); 

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

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
#include <stdbool.h>
//Static Functions
void QuickGameConfirm_DrawBackButton(void);
void QuickGameConfirm_CheckButtonPress(void);
void QuickGameConfirm_DrawInfoBoxes(void);
Rectangle GetLeftInfoBoxDimensions(const float screenWidth, float screenHeight);
Rectangle GetRightInfoBoxDimensions(const float screenWidth, float screenHeight);
void PopulateTeamSummaryInfoBox(const TeamData *teamData, const Rectangle *infoBox);
void QuickGameConfirm_LoadRosters(void);
//Static Vars
Button quickGameConfirmBackButton;
float randomColorHue = 0.0f;
bool quickGameConfirm_PlayerAndCpuRostersLoaded = false;
//Error Codes
typedef enum {
	ERROR_NONE,
	ERROR_GLOBAL_ROSTER_POINTER,
	ERROR_TEAM_ID,
	ERROR_ROSTER_FILE,
	ERROR_GLOBAL_ROSTER_COUNT,
	ERROR_COUNT
} LoadRosterErrorCode;

//Function Definitions

LoadRosterErrorCode QuickGameConfirm_LoadRosters(void)
{
	//Confirm both rosters are pointing to NULL
	if (griddy.playerRoster != NULL) {
		TraceLog(LOG_ERROR, "ERROR: playerRoster not NULL QGC_LoadRosters()");
		return ERROR_GLOBAL_ROSTER_POINTER;
	}
	if (griddy.cpuRoster != NULL) {
		TraceLog(LOG_ERROR, "ERROR: cpuRoster not NULL QGC_LoadRosters()");
		return ERROR_GLOBAL_ROSTER_POINTER;
	}
	//player first
	//figure out which team is playerTeam (teamID)
	if (griddy.playerTeam < TEAM_NONE + 1 || griddy.playerTeam >= TEAM_COUNT)
	{
		TraceLog(LOG_ERROR, "ERROR: playerTeamID OOB");
		return ERROR_TEAM_ID;
	}
	teamData = GetTeamData(griddy.playerTeam);
	//designate the file to load roster from
	char rosterFileName[32] = {0};
	snprintf(rosterFileName, sizeof(rosterFileName), "%s.roster", teamData->blueprint->name);
	FILE *rosterFile = fopen(rosterFileName, "rb");
	if (rosterFile == NULL) {
		TraceLog(LOG_ERROR, "ERROR: rosterFile does not exist!");
		return ERROR_ROSTER_FILE;
	}
	//count the numLines in file
	if (griddy.playerRosterCount != 0) {
		TraceLog(LOG_ERROR, "ERROR: playerRosterCount not 0!");
		fclose (rosterFile);
		return ERROR_GLOBAL_ROSTER_COUNT;
	}
	char stringBuffer[256] = {0};
	int numLines = 0;
	while (fgets(stringBuffer, 256, rosterFile)) {
		numLines++;
	}
	//save numLines into global ctx (to use in free function later on!)
	griddy.playerRosterCount = numLines;
	//rewind file pointer
	rewind(rosterFile);
	//create a player roster[] array of size numLines
	//global playerRoster points there
	griddy.playerRosterFile = calloc(numLines, sizeof(Player));
	//for each line in the file copy that data into roster[i]
	int i=0;
	while (i<numLines && fgets(stringBuffer, 256, rosterFile)) {
	//after it's all done close the file
	fclose (rosterFile);
	//that should be it - abstraction for cpu roster
	//set RostersLoaded bool to true so we don't do this to infinity and beyond
	//return all happy (dont forget to free this memory somewhere!)
	return ERROR_NONE;

}

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
	if (teamData->blueprint->id == griddy.playerTeam) {
		DrawTextInBox("Player Team", &targetTextBox);
	} else {
		DrawTextInBox("CPU Team", &targetTextBox);
	}
	//Seperator line
	DrawLine((int)infoBox->x, (int)targetTextBox.y + (int)targetTextBox.height, (int)infoBox->x + (int)infoBox->width, (int)targetTextBox.y + (int)targetTextBox.height, BLACK);

	//Team Name - 20%
	const char *teamName = teamData->blueprint->name;
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
	//If roster not loaded, load rosters
	if (!quickGameConfirm_PlayerAndCpuRostersLoaded) {
		LoadRosterErrorCode = ERROR_NONE;
		LoadRosterErrorCode = QuickGameConfirm_LoadRosters();
		if (LoadRosterErrorCode != ERROR_NONE) {
			TraceLog(LOG_ERROR, "ERROR: %d\nRosters Not Loaded!", LoadRosterErrorCode);
			griddy.state = MAIN_GAME_STATE_MAIN_MENU;
			return;
		}
	}
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

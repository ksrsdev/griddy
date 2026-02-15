#include "button.h"
#include "global.h"
#include "init.h"
#include "player.h"
#include "quick_game_confirm.h"
#include "raylib.h"
#include "roster.h"
#include "team.h"
#include "team_select.h"
#include "text.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum {
	ERROR_NONE,
	ERROR_GLOBAL_ROSTER_POINTER,
	ERROR_TEAM_ID,
	ERROR_ROSTER_FILE,
	ERROR_GLOBAL_ROSTER_COUNT,
	ERROR_ALLOCATION,
	ERROR_COUNT
} LoadRosterErrorCode;
//Static Functions
void QuickGameConfirm_DrawBackButton(void);
void QuickGameConfirm_CheckButtonPress(void);
void QuickGameConfirm_DrawInfoBoxes(void);
Rectangle GetLeftInfoBoxDimensions(const float screenWidth, float screenHeight);
Rectangle GetRightInfoBoxDimensions(const float screenWidth, float screenHeight);
void PopulateTeamSummaryInfoBox(const TeamData *teamData, const Rectangle *infoBox);
LoadRosterErrorCode QuickGameConfirm_LoadBothRosters(void);
LoadRosterErrorCode QuickGameConfirm_LoadRoster(char *fileName, Player **roster, long unsigned int *rosterCount);
//Static Vars
Button quickGameConfirmBackButton;
Button quickGameConfirm_PlayerRosterButton;
Button quickGameConfirm_CPURosterButton;
float randomColorHue = 0.0f;
bool quickGameConfirm_PlayerAndCpuRostersLoaded = false;
//Error Codes

//Function Definitions

LoadRosterErrorCode QuickGameConfirm_LoadRoster(char *fileName, Player **roster, long unsigned int *rosterCount)
{
	FILE *rosterFile = fopen(fileName, "rb");
	if (rosterFile == NULL) {
		TraceLog(LOG_ERROR, "ERROR: rosterFile does not exist!");
		return ERROR_ROSTER_FILE;
	}
	//confirm rosterCount was reset to 0
	if (*rosterCount != 0) {
		TraceLog(LOG_ERROR, "ERROR: rosterCount not 0!");
		fclose (rosterFile);
		return ERROR_GLOBAL_ROSTER_COUNT;
	}
	//count the numLines in file
	char stringBuffer[256] = {0};
	int numLines = 0;
	while (fgets(stringBuffer, sizeof(stringBuffer), rosterFile)) {
		numLines++;
	}
	//save numLines into global ctx (to use in free function later on!)
	*rosterCount = (long unsigned int)numLines;
	//rewind file pointer
	rewind(rosterFile);
	//create a player roster[] array of size numLines
	//global playerRoster points there
	*roster = calloc((long unsigned int)numLines, sizeof(Player));
	//confirm allocation success
	if (*roster == NULL) {
		return ERROR_ALLOCATION;
	}
	//for each line in the file copy that data into roster[i]
	for (int i=0; i<(int)numLines; i++) {
		if (fgets(stringBuffer, sizeof(stringBuffer), rosterFile)) {
			int tempPos;
			int dataLoaded = sscanf(stringBuffer, "%31[^|]|%31[^|]|%d|%d|%d", 
					(*roster)[i].firstName,
					(*roster)[i].lastName,
					&tempPos,
					&(*roster)[i].number,
					&(*roster)[i].overall);
			//Data loaded incorrectly, return ERROR
			if (dataLoaded != 5) {
				TraceLog(LOG_INFO, "Player on line %d Loaded Incorrectly", i);
				fclose(rosterFile);
				free(*roster);
				*roster = NULL;
				return ERROR_ROSTER_FILE;
			}
			//Data loaded correctly - handle tempPos
			(*roster)[i].position = (PlayerPosition)tempPos;
		}
		//Fgets read incorrectly, return ERROR
		else {
			TraceLog(LOG_INFO, "fgets returned NULL at line %d", i);
			free(*roster);
			*roster = NULL;
			fclose(rosterFile);
			return ERROR_ROSTER_FILE;
		}
	}
	//after it's all done close the file
	fclose (rosterFile);
	return ERROR_NONE;
}

LoadRosterErrorCode QuickGameConfirm_LoadBothRosters(void)
{
	LoadRosterErrorCode errorCheck = ERROR_NONE;
	//Load Player Roster
	if (ctx.playerRoster != NULL) {
		TraceLog(LOG_ERROR, "ERROR: playerRoster not NULL QGC_LoadRosters()");
		return ERROR_GLOBAL_ROSTER_POINTER;
	}
	else {
		if (ctx.playerTeamId <= TEAM_NONE + 1 || ctx.playerTeamId >= TEAM_COUNT)
		{
			TraceLog(LOG_ERROR, "ERROR: playerTeamID OOB");
			return ERROR_TEAM_ID;
		}
		const TeamData *teamData = GetTeamData(ctx.playerTeamId);
		//designate the file to load roster from
		char rosterFileName[32] = {0};
		snprintf(rosterFileName, sizeof(rosterFileName), "%s.roster", teamData->blueprint->name);
		errorCheck = QuickGameConfirm_LoadRoster(rosterFileName, &ctx.playerRoster, &ctx.playerRosterCount);
		if (errorCheck != ERROR_NONE) {
			return errorCheck;
		}
	}
	//Load CPU Roster
	if (ctx.cpuRoster != NULL) {
		TraceLog(LOG_ERROR, "ERROR: cpuRoster not NULL QGC_LoadRosters()");
		return ERROR_GLOBAL_ROSTER_POINTER;
	}
	else {
		if (ctx.cpuTeamId <= TEAM_NONE || ctx.cpuTeamId >= TEAM_COUNT)
		{
			TraceLog(LOG_ERROR, "ERROR: playerTeamID OOB");
			return ERROR_TEAM_ID;
		}
		const TeamData *teamData = GetTeamData(ctx.cpuTeamId);
		//designate the file to load roster from
		char rosterFileName[32] = {0};
		snprintf(rosterFileName, sizeof(rosterFileName), "%s.roster", teamData->blueprint->name);
		errorCheck = QuickGameConfirm_LoadRoster(rosterFileName, &ctx.cpuRoster, &ctx.cpuRosterCount);
		if (errorCheck != ERROR_NONE) {
			return errorCheck;
		}
	}
	//set RostersLoaded bool to true so we don't do this to infinity and beyond
	quickGameConfirm_PlayerAndCpuRostersLoaded = true;
	//return all happy (dont forget to free this memory somewhere!)
	return ERROR_NONE;
}

void InitQuickGameConfirmButtons(void)
{
	quickGameConfirmBackButton = MakeButton("<- BACK", RED);
	quickGameConfirm_PlayerRosterButton = MakeButton("ROSTER", GRAY);
	quickGameConfirm_CPURosterButton = MakeButton("ROSTER", GRAY);
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
	targetTextBox.x = infoBox->x + ((infoBox->width - targetTextBox.width) / 2);
	//height is determined by the section (title 20, name 20, players 50/3, roster 10)
	targetTextBox.y = infoBox->y;
	targetTextBox.height = infoBox->height / 5;
	
	//Title - top 20%
	if (teamData->blueprint->id == ctx.playerTeamId) {
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
	//Which roster to display players from
	Player *roster = NULL;
	long unsigned int count = 0;
	Button *button = NULL;
	if (teamData->blueprint->id == ctx.playerTeamId)  {
		roster = ctx.playerRoster;
		count = ctx.playerRosterCount;
		button = &quickGameConfirm_PlayerRosterButton;
	} else if (teamData->blueprint->id == ctx.cpuTeamId)  {
		roster = ctx.cpuRoster;
		count = ctx.cpuRosterCount;
		button = &quickGameConfirm_CPURosterButton;
	} else {
		TraceLog(LOG_ERROR, "teamData->id oob!");
		return;
	}
	//Sort roster array by ovr
	qsort(roster, count, sizeof(Player), ComparePlayers_ReturnLargerOvr);
	//Lets start with just text before I draw the player circle
	//The entire player section should be 50 %
	targetTextBox.height = (infoBox->height / 2.0f) / 3.0f;
	targetTextBox.width -= targetTextBox.height;
	targetTextBox.x += targetTextBox.height;
	for (int i=0; i<3; i++) {
		//Text side first
		targetTextBox.y += targetTextBox.height;
		Player player = roster[i];
		char targetText[128] = {0};
		snprintf(targetText, sizeof(targetText), "%s %s %s %d", player.firstName, player.lastName, positionNames[player.position], player.overall);
		DrawTextInBoxColor(targetText, &targetTextBox, &teamColor);	
		//Player Circles
		float radius = targetTextBox.height / 2.5f;
		//center point of the circle:
		Vector2 centerPoint = {0};
		centerPoint.x = infoBox->x + (infoBox->width - (infoBox->width - (infoBox->width / 10)));
		centerPoint.y = targetTextBox.y + (targetTextBox.height / 2);
		DrawCircleV(centerPoint, radius, teamColor);
		//Number on Circle
		snprintf(targetText, sizeof(targetText), "%d", player.number);
		DrawTextInCircle(targetText, centerPoint, radius);
	}
	//Roster Button
	button->rec.height = infoBox->height / 10.0f;
	button->rec.width = infoBox->width / 5.0f;
	button->rec.x = infoBox->x + ((infoBox->width - button->rec.width) / 2.0f);
	button->rec.y = targetTextBox.y + targetTextBox.height + (infoBox->height / 66.66f);
	DrawSingleButton(button);
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
	const TeamData *playerTeamData = GetTeamData(ctx.playerTeamId);
	const TeamData *cpuTeamData = GetTeamData(ctx.cpuTeamId);
	//Do the actual text rendering
	PopulateTeamSummaryInfoBox(playerTeamData, &playerInfoBox); 
	PopulateTeamSummaryInfoBox(cpuTeamData, &cpuInfoBox); 
	//Populate with text



	//Below should be put into the Populate() function


	//Depth Chart Button

}

void QuickGameConfirm_UnloadRosters(void) 
{
	if (ctx.playerRoster != NULL) {
		UnloadRoster(&ctx.playerRoster, &ctx.playerRosterCount);
	}
	if (ctx.cpuRoster != NULL) {
		UnloadRoster(&ctx.cpuRoster, &ctx.cpuRosterCount);
	}
	quickGameConfirm_PlayerAndCpuRostersLoaded = false;
}

void QuickGameConfirm_CheckButtonPress(void)
{
	//Back button
	if (CheckSingleButtonForButtonPress(&quickGameConfirmBackButton)) {
		QuickGameConfirm_UnloadRosters();
		InitTeamSelect();
		ctx.state = MAIN_GAME_STATE_QUICK_GAME_PLAYER_TEAM_SELECT;
	}
}

void DrawQuickGameConfirm(void)
{
	//Clear
	ClearBackground(RAYWHITE);
	//If roster not loaded, load rosters
	if (!quickGameConfirm_PlayerAndCpuRostersLoaded) {
		LoadRosterErrorCode loadRosterErrorCode = ERROR_NONE;
		loadRosterErrorCode = QuickGameConfirm_LoadBothRosters();
		if (loadRosterErrorCode != ERROR_NONE) {
			TraceLog(LOG_ERROR, "ERROR: %d\nRosters Not Loaded!", loadRosterErrorCode);
			ctx.state = MAIN_GAME_STATE_MAIN_MENU;
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

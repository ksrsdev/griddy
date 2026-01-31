#include "button.h"
#include "init.h"
#include "main.h"
#include "quick_game_confirm.h"
#include "raylib.h"
#include "text.h"

void QuickGameConfirm_DrawBackButton(void);
void QuickGameConfirm_CheckButtonPress(void);
void QuickGameConfirm_DrawInfoBoxes(void);

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

void QuickGameConfirm_DrawInfoBoxes(void)
{
	//Outlines First
	float screenWidth = (float)GetScreenWidth();
	float screenHeight = (float)GetScreenHeight();
	Rectangle playerInfoBoxOutline, cpuInfoBoxOutline;
	//player info box assingment
	playerInfoBoxOutline.x = screenWidth / 10.0f;
	playerInfoBoxOutline.y = screenHeight / 5.0f;
	playerInfoBoxOutline.width = screenWidth / 3.0f;
	playerInfoBoxOutline.height = screenHeight * (0.6f);
	//cpu info box assingment
	cpuInfoBoxOutline.x = screenWidth - (screenWidth / 10.0f) - playerInfoBoxOutline.width;
	cpuInfoBoxOutline.y = screenHeight / 5.0f;
	cpuInfoBoxOutline.width = screenWidth / 3.0f;
	cpuInfoBoxOutline.height = screenHeight * (0.6f);
	DrawRectangleLinesEx(playerInfoBoxOutline, 2.0, BLACK);
	DrawRectangleLinesEx(cpuInfoBoxOutline, 2.0, BLACK);
	//Populate with text

	//Title
	DrawInfoBoxTitleText("Player Team", playerInfoBoxOutline);
	DrawInfoBoxTitleText("CPU Team", cpuInfoBoxOutline);

	//Team Name
	//char *playerTeam, *cpuTeam;
	//Color playerColor, cpuColor;
	//handle random color hue
	//if (teamSelectRandomButtonHue > 360.0f) {
	//	teamSelectRandomButtonHue = 0.0f;
	//} else {
	//	teamSelectRandomButtonHue++;
	//}
	//switch (playerTeamSelected) {
	//	case TEAM_SELECTED_NONE:
	//		playerTeam = "INVALID SELECTION!";
	//		playerColor = BLACK;
	//		break;
	//	case TEAM_SELECTED_RANDOM:
	//		playerTeam = "Random Team";
	//		playerColor = ColorFromHSV(randomColorHue, 1.0f, 1.0f);
	//		break;

	//}
}

void QuickGameConfirm_CheckButtonPress(void)
{
	//Back button
	if (CheckSingleButtonForButtonPress(&quickGameConfirmBackButton)) {
		InitTeamSelect();
		mainGameState = MAIN_GAME_STATE_QUICK_GAME_PLAYER_TEAM_SELECT;
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

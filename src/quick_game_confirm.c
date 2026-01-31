#include "button.h"
#include "init.h"
#include "main.h"
#include "quick_game_confirm.h"
#include "raylib.h"
#include "text.h"

void QuickGameConfirm_DrawBackButton(void);

Button quickGameConfirmBackButton;

void InitQuickGameConfirmButtons(void)
{
	quickGameConfirmBackButton = MakeButton("<- BACK", RED);
}

void QuickGameConfirm_DrawBackButton(void)
{
	RepositionSingleButton_BottomLeft(&quickGameConfirmBackButton);
	DrawSingleButton(&quickGameConfirmBackButton);
}

void DrawQuickGameConfirm(void)
{
	//Clear
	ClearBackground(RAYWHITE);
	//Title
	DrawMenuTitleText("Quick Game Confirm");
	//Info Boxes
//	QuickGameConfirmDrawInfoBoxes();
	//Player info box
	//CPU info box
	//home team graphic
	//weather option
	//start button
	//back button
	QuickGameConfirm_DrawBackButton();
	//check button press
}

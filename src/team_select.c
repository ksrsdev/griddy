#include "raylib.h"
#include "team_select.h"

void TeamSelectDrawTitleText(void);


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

void DrawQuickGameTeamSelect(void)
{
	//Clear
	ClearBackground(RAYWHITE);
	//Select Text
	TeamSelectDrawTitleText();
	//Text Box
	//back button
	//TeamSelectDrawBackButton();
	//team buttons
	//check button press
}

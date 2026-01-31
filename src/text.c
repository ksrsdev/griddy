#include "main.h"
#include "raylib.h"
#include "text.h"

void DrawMainMenuSplash(Vector2 titleTextPos, Vector2 titleTextSize);

int splashTextIndex = -1;
int splashTextSizeModifierState = 0;

//Splash texts
const char* splashTextArray[SPLASH_TEXT_COUNT] = 
{
	//10 characters +/- otherwise it gets choppy
	"BASED",
	"WELCOME :D",
	"SUGAR FREE",
	"GRIDIRON!",
	"VITAMIN D3",
};

void DrawMenuTitleText(const char *titleText)
{
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
	if (mainGameState == MAIN_GAME_STATE_MAIN_MENU) {
		DrawMainMenuSplash(textPos, textSize);
	}
}

void DrawMainMenuSplash(Vector2 titleTextPos, Vector2 titleTextSize)
{
	//Just like Minecraft
	//Random string, tilted and pulsing
	//center of string should be the lower right corner of the title text
	//rotate it up 45? degrees   
	
	//Pick random splash if needed
	if (splashTextIndex == -1) {
		splashTextIndex = GetRandomValue(0, 4);
	}
	const char* splashText = splashTextArray[splashTextIndex];
	//set font size
	//Size should oscilate from 1/3 to 1/2 back and forth but remain centered
	//1 second from 1/3 to 1/2
	//1 second from 1/2 to 1/3
	if (splashTextSizeModifierState == 120) {
		splashTextSizeModifierState = 0;
	} else {
		splashTextSizeModifierState++;
	}
	float splashTextSizeModifier = (float)splashTextSizeModifierState;
	if (splashTextSizeModifier > 60) {
		splashTextSizeModifier = 60 - (splashTextSizeModifier - 60);
	}
	splashTextSizeModifier = (1.0f/3.0f) + (splashTextSizeModifier / 360.0f);
	float fontSize = 1;
	Vector2 splashTextSize = MeasureTextEx(GetFontDefault(), splashText, fontSize, 1.0f);
	while (splashTextSize.x < (titleTextSize.x * splashTextSizeModifier)  && splashTextSize.y < (titleTextSize.y * splashTextSizeModifier)) {
		fontSize++;
		splashTextSize = MeasureTextEx(GetFontDefault(), splashText, fontSize, 1.0f);
	}
	//set origin to midpoint of the text string
	Vector2 splashTextOrigin;
	splashTextOrigin.x = splashTextSize.x / 2;
	splashTextOrigin.y = splashTextSize.y / 2;
	//the point on the screen to draw the origin = bottom right corner of title
	Vector2 splashTextPos;
	splashTextPos.x = titleTextPos.x + titleTextSize.x;
	splashTextPos.y = titleTextPos.y + titleTextSize.y - (titleTextSize.y / 6);
	//determine midpoint for rotation
	float rotation = -30;
//	DrawText(splashTextArray[splashTextIndex], 45, 45, 45, RED);
	DrawTextPro(GetFontDefault(), splashText, splashTextPos, splashTextOrigin, rotation, fontSize, 1.0f, RED);
}

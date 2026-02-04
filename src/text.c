#include "global.h"
#include "raylib.h"
#include "text.h"


int CalculateMaxFontSize(const char *text, const Vector2 *bounds);
Vector2 CalculateCenterTextPos(const char *text, const Rectangle *textBoxRec, const int fontSize);

void DrawMainMenuSplash(Vector2 titleTextPos, Vector2 titleTextSize);

static int splashTextIndex = -1;
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


Vector2 CalculateCenterTextPos(const char *text, const Rectangle *textBoxRec, const int fontSize)
{
	Vector2 textSize, textPos;
	textSize = MeasureTextEx(GetFontDefault(), text, (float)fontSize, 1.0f);
	textPos.x = textBoxRec->x + ((textBoxRec->width - textSize.x) / 2);
	textPos.y = textBoxRec->y + ((textBoxRec->height - textSize.y) / 2);
	return textPos;
}

int CalculateMaxFontSize(const char *text, const Vector2 *bounds)
{	int fontSize = 1;
	Vector2 textSize = MeasureTextEx(GetFontDefault(), text, (float)fontSize, 1.0f);
	while (textSize.x < bounds->x && textSize.y < bounds->y) {
		fontSize++;
		textSize = MeasureTextEx(GetFontDefault(), text, (float)fontSize, 1.0f);
	}
	return fontSize;
}

void DrawTextInBoxColor(const char *text, const Rectangle *textBoxRec, const Color *color)
{
	Vector2 textBoxSize;
	textBoxSize.x = textBoxRec->width;
	textBoxSize.y = textBoxRec->height;
	int fontSize = CalculateMaxFontSize(text, &textBoxSize);
	Vector2	textPos = CalculateCenterTextPos(text, textBoxRec, fontSize);
	DrawTextEx(GetFontDefault(), text, textPos, (float)fontSize, 1.0f, *color);
}

//Draw a centered title on entire screen (1/12 yMargin and 1/3 xMargin)
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
	if (griddy.state == MAIN_GAME_STATE_MAIN_MENU) {
		DrawMainMenuSplash(textPos, textSize);
	}
}

//Draw a centered title for an info box (1/20 yMargin 1/3 xMargin)
void DrawInfoBoxTitleText(const char *titleText, const Rectangle *infoBox)
{
	float marginX = infoBox->width / 3.0f;
	float marginY = infoBox->height / 20.0f;
	Vector2 textSize, textBox, textPos;
	textBox.x = infoBox->width - (2 * marginX);
	textBox.y = marginY * 4;
	textPos.x = infoBox->x + marginX;
	textPos.y = infoBox->y + marginY;
	int fontSize = 1;
	textSize = MeasureTextEx(GetFontDefault(), titleText, (float)fontSize, 1.0f);
	while (textSize.x < textBox.x && textSize.y < textBox.y) {
		fontSize++;
		textSize = MeasureTextEx(GetFontDefault(), titleText, (float)fontSize, 1.0f);
	}
	DrawTextEx(GetFontDefault(), titleText, textPos, (float)fontSize, 1.0f, BLACK);
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

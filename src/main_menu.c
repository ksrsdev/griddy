#include "button.h"
#include "startup.h"
#include "main.h"
#include "main_menu.h"
#include "raylib.h"

//functions
void MainMenuDrawButtons(void);
void DrawMainMenuTitle(void);
void DrawMainMenuSplash(Vector2 titleTextPos, Vector2 titleTextSize);
void DrawMainMenuVersionNumber(void);
void MainMenuCheckButtonPress(void);
//variables
bool buttonsMainMenuReady = false;
int splashTextIndex = -1;
int splashTextSizeModifierState = 0;
//button array
Button MainMenuButtons[MAIN_MENU_BUTTON_COUNT];
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

//definitions
void DrawMainMenuTitle(void)
{
	//MarginX same as buttons - 33
	//Margin Y should be the middle of the top margin
	//top margin is 33
	//33 / 2 ~= 16.5
	//so it should be height about 16.5 of the screen
	//with a y margin of 8.25, yes?
	char *titleText = "GRIDDY!";
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
	DrawMainMenuSplash(textPos, textSize);
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

void DrawMainMenuVersionNumber(void)
{
	char *versionText = "ALPHA A.1";
	float screenWidth = (float)GetScreenWidth();
	float screenHeight = (float)GetScreenHeight();
	float marginX = screenWidth / 20.0f;
	float marginY = screenHeight / 20.0f;
	Vector2 textSize, textPos;
	textPos.x = marginX;
	textPos.y = screenHeight - marginY;
	int fontSize = 1;
	textSize = MeasureTextEx(GetFontDefault(), versionText, (float)fontSize, 1.0f);
	while (textSize.x < screenWidth / 20.0f && textSize.y < screenHeight / 20.0f) {
		fontSize++;
		textSize = MeasureTextEx(GetFontDefault(), versionText, (float)fontSize, 1.0f);
	}
	DrawTextEx(GetFontDefault(), versionText, textPos, (float)fontSize, 1.0f, BLACK);
}

void InitMainMenuButtons() 
{
	MainMenuButtons[0] = MakeButton("QUICK GAME", GRAY);
	MainMenuButtons[1] = MakeButton("NEW GAME", GRAY);
	MainMenuButtons[2] = MakeButton("LOAD GAME", GRAY);
	MainMenuButtons[3] = MakeButton("RELOAD", GRAY);
	MainMenuButtons[4] = MakeButton("EXIT", GRAY);
}

void MainMenuDrawButtons(void)
{
	if (IsWindowResized()) {
		buttonsMainMenuReady = false;
	}
	if (!buttonsMainMenuReady) {
		//Resize et Reposition
		RepositionButtonArray_CenteredVertical(MainMenuButtons, MAIN_MENU_BUTTON_COUNT, 17, 33);
		//Draw
		DrawButtonArray(MainMenuButtons, MAIN_MENU_BUTTON_COUNT);
	}
}

void MainMenuCheckButtonPress(void)
{
	int press = CheckButtonArrayForButtonPress(MainMenuButtons, MAIN_MENU_BUTTON_COUNT);
	if (press == -1) {
		return;
	}
	switch (press) {
		case 0:
			mainGameState = MAIN_GAME_STATE_QUICK_GAME_TEAM_SELECT;
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			StartupInitVars();
			splashTextIndex = -1;
			mainGameState = MAIN_GAME_STATE_STARTUP;
			break;
		case 4:
			gameRunning = false;
			break;
		default:
			TraceLog(LOG_INFO, "press OOB");
			break;
	}

}

void DrawMainMenu(void)
{
	//Clear
	ClearBackground(RAYWHITE);
	//Draw Griddy and splash - should be shaped via screen size
	DrawMainMenuTitle();
	//Draw version number
	DrawMainMenuVersionNumber();
	//Draw Buttons - center 33% of the screen
	MainMenuDrawButtons();
	//Check Button Press
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		MainMenuCheckButtonPress(); 
	}
}

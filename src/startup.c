#include "startup.h"
#include "init.h"
#include "main.h"
#include "main_menu.h"
#include "raylib.h"
#include "team_select.h"
#include "test_playground.h"

static const char *startupMessage = "GRIDDY!";
static double startTime = -1;
static float totalStartupTime = 1.0;
static float fadeStartupTime = 0.5;
static bool StartupVarsInitialized = false;


//display text for x seconds then increment mainGameState
int DrawStartup(void)
{
	if (!StartupVarsInitialized) {
		InitStartup();
		startTime = -1;
		StartupVarsInitialized = true;
	}
    if (startTime < 0)
        startTime = GetTime();
    double elapsedTime = GetTime() - startTime;
	float fontSize;           //text font size
	Vector2 textSize, textPos;
    //Set centered text font size to be 33% of the screen size
    fontSize = (float)GetScreenHeight() / 3.0f;
    if ((float)elapsedTime < fadeStartupTime)
    {
        ClearBackground(BLACK);
        fontSize *= (float)elapsedTime / fadeStartupTime;
    } else
        ClearBackground(WHITE);
    //Determine text size (we need to know how much space it will take up to determine how to center it on the screen size)
    textSize = MeasureTextEx(GetFontDefault(), startupMessage, fontSize, 1.0f);
    //Center Text Box// Calculate centered position
    textPos.x = ((float)GetScreenWidth()  - textSize.x) / 2.0f;
    textPos.y = ((float)GetScreenHeight() - textSize.x / 5) / 2.0f;
    if ((float)elapsedTime < fadeStartupTime) {
		DrawTextEx(GetFontDefault(), startupMessage, textPos, fontSize, 1.0f, WHITE);
	}
    else {
		DrawTextEx(GetFontDefault(), startupMessage, textPos, fontSize, 1.0f, BLACK);

	}
    if ((float)elapsedTime > totalStartupTime) {
        mainGameState = MAIN_GAME_STATE_QUICK_GAME_PLAYER_TEAM_SELECT;
	}
    return 0;
}

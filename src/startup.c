#include "raylib.h"
#include "main.h"

static const char *startupMessage = "GRIDDY!";
static double startTime = -1;
static float totalStartupTime = 1.0;
static float fadeStartupTime = 0.5;

//display text for x seconds then increment mainGameState
int DrawStartup(void)
{
    if (startTime < 0)
        startTime = GetTime();
    double elapsedTime = GetTime() - startTime;
	int fontSize;           //text font size
	float textPosX, textPosY; //text location
	int textWidth;         //the physical size the text takes up on the screen
    //Set centered text font size to be 33% of the screen size
    fontSize = GetScreenHeight() / 3;
    if (elapsedTime < fadeStartupTime)
    {
        ClearBackground(BLACK);
        fontSize *= elapsedTime / fadeStartupTime;
    } else
        ClearBackground(WHITE);
    //Determine text size (we need to know how much space it will take up to determine how to center it on the screen size)
    textWidth = MeasureText(startupMessage, fontSize);
    //Center Text Box// Calculate centered position
    textPosX = (GetScreenWidth()  - textWidth) / 2.0f;
    textPosY = (GetScreenHeight() - textWidth / 5) / 2.0f;
    if (elapsedTime < fadeStartupTime)
        DrawText(startupMessage, textPosX, textPosY, fontSize, WHITE);
    else
        DrawText(startupMessage, textPosX, textPosY, fontSize, BLACK);
    if (elapsedTime > totalStartupTime)
        mainGameState = MAIN_GAME_STATE_TEST_PLAYGROUND;
    return 0;
}

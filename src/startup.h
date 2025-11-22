#include "raylib.h"

static const char *startupMessage = "GRIDDY!";

//display text for x seconds then increment mainGameState
int drawStartup(void)
{
	
	int framesCounter = 0;    //hold the startup for some time
	float fontSize;           //text font size
	float textPosX, textPosY; //text location
	Vector2 textSize;         //the physical size the text takes up on the screen
	while (framesCounter < 420)
	{
		BeginDrawing();
		ClearBackground(BLACK);
		//Set centered text font size to be 33% of the screen size
		fontSize = GetScreenHeight() / 3;
		//Determine text size (we need to know how much space it will take up to determine how to center it on the screen size)
		textSize = MeasureText(startupMessage, fontSize);
		//Center Text Box// Calculate centered position
        textPosX = (GetScreenWidth()  - textSize.x) / 2.0f;
        textPosY = (GetScreenHeight() - textSize.y) / 2.0f;
		DrawText(startupMessage, textPosX, textPosY, fontSize / framesCounter, WHITE);
		framesCounter++;
	}
	//inc mainGameState
	mainGameState = MAIN_GAME_STATE_TEST_PLAYGROUND;
}

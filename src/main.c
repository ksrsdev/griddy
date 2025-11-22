#include "raylib.h"
    
const int screenWidth = 960;
const int screenHeight = 540;

bool MouseOverButton(Texture2D buttonTexture)
{
    Vector2 mousePos = GetMousePosition();
    Rectangle buttonRect;

    //setup buttonRect
    buttonRect.x = screenWidth - buttonTexture.width;
    buttonRect.y = screenHeight - buttonTexture.height;
    buttonRect.width = buttonTexture.width;
    buttonRect.height = buttonTexture.height;


    if (CheckCollisionPointRec(mousePos, buttonRect))
        return true;
    else
        return false;
}


int main(void)
{

    int displayButton = 0;

	int mainGameState = MAIN_GAME_STATE_STARTUP; //tracks what screen the game is displaying

    InitWindow(screenWidth, screenHeight, "Griddy");
    SetTargetFPS(60);
    
    while (!WindowShouldClose())
    {
		//Here is the entire game logic past this while loop game closes
		//First show startup screen
		//Next should be main menu BUT I'm doing a test playground first
		switch (mainGameState)
		{
			case MAIN_GAME_STATE_STARTUP:
				drawStartup();
			case MAIN_GAME_STATE_MAIN_MENU:
			case MAIN_GAME_STATE_TEST_PLAYGROUND:
				drawTestPlaygroundSetup();
		}
    }

    //Unload Textures
    UnloadTexture(pressMeButtonTexture);
    CloseWindow();
    return 0;
}

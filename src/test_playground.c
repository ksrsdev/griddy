#include "raylib.h"

int drawTestPlaygroundSetup(void)
{
    // Texture loading
    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)
    Texture2D pressMeButtonTexture = LoadTexture("graphics/press_me.png");   
	
	//draw test playground on a loop
	while (mainGameState == MAIN_GAME_STATE_TEST_PLAYGROUND)
	{
		drawTestPlayground();
	}
	
	//after loop ends unload texture
	UnloadTexture(pressMeButtonTexture);	
	return 0;

}

int drawTestPlayground(void)
{
	//Check if button is being pressed
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && MouseOverButton(pressMeButtonTexture))
		displayButton = 1;
	BeginDrawing();
	ClearBackground(RAYWHITE);
	DrawText("Hello raylib on macOS!", 240, 240, 40, DARKGRAY);
	DrawText("Moar Text Here", 420, 420, 15, RED);
	if (displayButton == 0)
		DrawTexture(pressMeButtonTexture, screenWidth - pressMeButtonTexture.width, screenHeight - pressMeButtonTexture.height, WHITE);
	DrawFPS(10, 10);
	EndDrawing();
}

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

    InitWindow(screenWidth, screenHeight, "Griddy");
    SetTargetFPS(60);
    
    // Texture loading
    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)
    Texture2D pressMeButtonTexture = LoadTexture("graphics/press_me.png");        
    while (!WindowShouldClose())
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

    //Unload Textures
    UnloadTexture(pressMeButtonTexture);
    CloseWindow();
    return 0;
}

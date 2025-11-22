#include "raylib.h"
#include "main.h"

//static functions
int  DrawTestPlayground(void);
bool IsTestButtonClickToHideBeingPressed(Vector2 mousePos);
void SetupTestButtonSizes(void);
void DrawTestPlayground_ButtonClickToHide(void);
void DrawTestPlaygroundMainMenuButton(void);
bool IsMainMenuButtonBeingPressed(Vector2 mousePos);

void CheckButtonPress_TestPlayground(void);

//static variables
int displayButtonClickToHide = 0;
Rectangle rec_TestButtonHideToClick;

static Rectangle MainMenuButton = {0};
static const char *MainMenuText = "MAIN";
static int MainMenuFontSize = 0;
static float MainMenuTextPosX = 0;
static float MainMenuTextPosY = 0;
static Vector2 MainMenuTextSize = {0};
static bool  buttonMainMenuSizeReady = false;


bool IsTestButtonClickToHideBeingPressed(Vector2 mousePos)
{
    if (CheckCollisionPointRec(mousePos, rec_TestButtonHideToClick))
        return true;
    else
        return false;
}

bool IsMainMenuButtonBeingPressed(Vector2 mousePos)
{
    if (CheckCollisionPointRec(mousePos, MainMenuButton))
        return true;
    else
        return false;
}

void CheckButtonPress_TestPlayground(void)
{
    Vector2 mousePos = GetMousePosition();
    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        return;
    //Click to hide button
    if (IsTestButtonClickToHideBeingPressed(mousePos))
        displayButtonClickToHide = 1;
    //Main Menu Button
    if (IsMainMenuButtonBeingPressed(mousePos))
        mainGameState = MAIN_GAME_STATE_STARTUP;
    return;
}

//set button sizes dynamically based on current screen size
void SetupTestButtonSizes(void)
{
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();
    rec_TestButtonHideToClick.width = screenWidth / 10;
    rec_TestButtonHideToClick.height = screenHeight / 10;
    rec_TestButtonHideToClick.x = screenWidth - (rec_TestButtonHideToClick.width * 2);
    rec_TestButtonHideToClick.y = screenHeight - (rec_TestButtonHideToClick.height * 2);
    return;
}

void DrawTestPlaygroundMainMenuButton(void)
{
    if (IsWindowResized())
        buttonMainMenuSizeReady = false;
    //resize menu button et text first time or if screen size has changed
    if (!buttonMainMenuSizeReady) 
    {
        //button rec
        MainMenuButton.width = GetScreenWidth() / 10;
        MainMenuButton.height = GetScreenHeight() / 10;
        MainMenuButton.x = MainMenuButton.width;
        MainMenuButton.y = GetScreenHeight() - (MainMenuButton.height * 2);
        //text
        //I want text perfect centered on Y and same width as button
        MainMenuFontSize = MainMenuButton.height * 0.6;
        MainMenuTextSize = MeasureTextEx(GetFontDefault(), MainMenuText, MainMenuFontSize, 1);
        
        //Check if text is wider than the button, if so scale it down
        while (MainMenuTextSize.x > MainMenuButton.width + (MainMenuButton.width - MainMenuTextSize.x))
        {
            MainMenuFontSize--; 
            MainMenuTextSize = MeasureTextEx(GetFontDefault(), MainMenuText, MainMenuFontSize, 0);
        }

        //Print Debug
        TraceLog(LOG_INFO, "\nFont Size: %d\nButton Width: %f\nButton Height: %f\nText Width: %f\nText Height: %f", MainMenuFontSize, MainMenuButton.width, MainMenuButton.height, MainMenuTextSize.x, MainMenuTextSize.y);
        MainMenuTextPosX = MainMenuButton.x + (MainMenuButton.width - MainMenuTextSize.x)/ 2;
        MainMenuTextPosY = MainMenuButton.y + (MainMenuButton.height - MainMenuTextSize.y) / 2;

        //menu button is correct size now
        buttonMainMenuSizeReady = true;
    }
    DrawRectangleRec(MainMenuButton, BLUE);
    if (MainMenuTextSize.x > MainMenuButton.width || MainMenuTextSize.y > MainMenuButton.height)
        DrawText(MainMenuText, MainMenuTextPosX, MainMenuTextPosY, MainMenuFontSize, RED);
    else
        DrawText(MainMenuText, MainMenuTextPosX, MainMenuTextPosY, MainMenuFontSize, BLACK);
    return;
}

int DrawTestPlaygroundSetup(void)
{
    // Texture loading
    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)
//    Texture2D pressMeButtonTexture = LoadTexture("graphics/press_me.png");  

	//draw test playground on a loop
	if (mainGameState == MAIN_GAME_STATE_TEST_PLAYGROUND)
		DrawTestPlayground();
	
	//after loop ends unload texture
//	UnloadTexture(pressMeButtonTexture);	
	return 0;

}

//Draw ClickToHide button
void DrawTestPlayground_ButtonClickToHide(void) {
    DrawRectangleRec(rec_TestButtonHideToClick, GREEN);
    return;
}

int DrawTestPlayground(void)
{
    //dynamically assign button sizes
    SetupTestButtonSizes();
	//Check if button is being pressed
    CheckButtonPress_TestPlayground();
	ClearBackground(RAYWHITE);
	DrawText("Handful of Rain", 240, 240, 40, DARKGRAY);
	DrawText("Moar Text Here", 420, 420, 15, RED);
	if (displayButtonClickToHide == 0)
        DrawTestPlayground_ButtonClickToHide();
    //main menu button
    DrawTestPlaygroundMainMenuButton();
	DrawFPS(10, 10);
    return 0;
}

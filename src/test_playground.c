#include "raylib.h"
#include "main.h"

//static functions
int  DrawTestPlayground(void);
bool IsTestButtonClickToHideBeingPressed(void);
void SetupTestButtonSizes(void);
void DrawTestPlayground_ButtonClickToHide(void);
void DrawTestPlaygroundMainMenuButton(void);

//static variables
int displayButton = 0;
Rectangle rec_TestButtonHideToClick;

static Rectangle buttonMainMenu = {0};
static const char *MainMenuText = "MAIN";
static int MainMenuFontSize = 0;
static float MainMenuTextPosX = 0;
static float MainMenuTextPosY = 0;
static int MainMenuTextWidth = 0;
static bool  buttonMainMenuSizeReady = false;


bool IsTestButtonClickToHideBeingPressed(void)
{
    Vector2 mousePos = GetMousePosition();
    if (CheckCollisionPointRec(mousePos, rec_TestButtonHideToClick) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        return true;
    else
        return false;
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
        buttonMainMenu.width = GetScreenWidth() / 10;
        buttonMainMenu.height = GetScreenHeight() / 10;
        buttonMainMenu.x = buttonMainMenu.width;
        buttonMainMenu.y = GetScreenHeight() - (buttonMainMenu.height * 2);
        //text
        //I want text perfect centered on Y and same width as button
        MainMenuFontSize = buttonMainMenu.height * 0.6;
        MainMenuTextWidth = MeasureText(MainMenuText, MainMenuFontSize);
        
        MainMenuTextPosX = buttonMainMenu.x + (buttonMainMenu.width - MainMenuTextWidth)/ 2;
        MainMenuTextPosY = buttonMainMenu.y + buttonMainMenu.height - MainMenuFontSize;

        //menu button is correct size now
        buttonMainMenuSizeReady = true;
        TraceLog(LOG_INFO, "\nbutton width: %f\ntext width: %d\nfont size: %d", buttonMainMenu.width, MainMenuTextWidth, MainMenuFontSize);
    }
    DrawRectangleRec(buttonMainMenu, BLUE);
    if (MainMenuTextWidth > buttonMainMenu.width)
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
    if (IsTestButtonClickToHideBeingPressed())
        displayButton = 1;
	ClearBackground(RAYWHITE);
	DrawText("Handful of Rain", 240, 240, 40, DARKGRAY);
	DrawText("Moar Text Here", 420, 420, 15, RED);
	if (displayButton == 0)
        DrawTestPlayground_ButtonClickToHide();
    //main menu button
    DrawTestPlaygroundMainMenuButton();
	DrawFPS(10, 10);
    return 0;
}

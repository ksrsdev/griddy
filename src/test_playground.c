#include "raylib.h"
#include "main.h"

//static functions
int  DrawTestPlayground(void);
bool IsTestButtonClickToHideBeingPressed(Vector2 mousePos);
void SetupTestButtonSizes(void);
void DrawTestPlayground_ButtonClickToHide(void);
void DrawTestPlaygroundMainMenuButton(void);
bool IsMainMenuButtonBeingPressed(Vector2 mousePos);
void DrawTestPlaygroundExitButton(void);

void TestPlaygroundCheckButtonPress(void);

typedef struct {
	Rectangle rect;
	const char* text;
	Color bg_color;
} Button;

static Button TestPlaygroundButtons[] = {
	{
		.text = "MAIN"
		.bg_color = BLUE;
	},
	{
		.text = "EXIT"
		.bg_color = RED;
	},
	{
		.text = "HIDE"
		.bg_color = GREEN;
	}
};

//static variables
int displayButtonClickToHide = 0;
Rectangle rec_TestButtonHideToClick;

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

void TestPlaygroundCheckButtonPress(void)
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

void TestPlaygroundDrawMainMenuButton(void)
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

void TestPlaygroundDrawExitButton(void) 
{
    if (IsWindowResized())
        buttonMainMenuSizeReady = false;

}

int DrawTestPlaygroundSetup(void)
{
	if (mainGameState == MAIN_GAME_STATE_TEST_PLAYGROUND)
		DrawTestPlayground();
	return 0;
}

//Draw ClickToHide button
void TestPlaygroundDrawButtonClickToHide(void) {
    DrawRectangleRec(rec_TestButtonHideToClick, GREEN);
    return;
}

void TestPlaygroundResizeButtons(void) {
	//for all in array:
	//set button w et h
	//set text font size
	//set text
	for (int i = 0; i < sizeof(TestPlaygroundButtons); i++) {
		TestPlaygroundButtons[i].rect.width = screenWidth / 10;
		TestPlaygroundButtons[i].rect.height = screenHeight / 10;
		TestPlaygroundButtons[i].rect.y = screenHeight - 2 * (screenHeight / 10);
		//x is tougher
		//0 quarter screen width - half button width
		//1 half screen width - half button width = centered
		//2 three quarder screen width - half button width
		TestPlaygroundDrawButtons[i].rect.x = (screenWidth / 4) + ((screenWidth / 4) * i) - TestPlaygroundButtons[i].rect.width;
	}
}

void TestPlaygroundDrawButtons(void) {
	//hide button
	if (displayButtonClickToHide == 0)
        TestPlaygroundDrawButtonClickToHide();
    //main menu button
    TestPlaygroundDrawMainMenuButton();
	//exit button
	TestPlaygroundDrawExitButton();
	// ^ Above is old method

	//Here is new method 
	
	//vars used to draw buttons
	int fontSize;
	Vector2 textSize, textPos;
	
	//First handle button resizes
	if (IsWindowResized()) {
		buttonMainMenuSizeReady = false;
	}
	if (!buttonMainMenuSizeReady) {
		TestPlaygroundResizeButtons();
	}
	//Then draw the button with text on it
	for (int i = 0; i < sizeof(TestPlaygroundButtons); i++) {
		DrawRectangleRec(TestPlaygroundButtons[i].rect, TestPlaygroundButtons[i].bg_color);
		fontSize = TestPlaygroundButtons[i].height * 0.6;

		
	return;
}

int DrawTestPlayground(void)
{
    //dynamically assign button sizes
    SetupTestButtonSizes();
	//check button press
    TestPlaygroundCheckButtonPress(); 
	//clear
	ClearBackground(RAYWHITE);
	//draw message text
	DrawText("Handful of Rain", 240, 240, 40, DARKGRAY);
	DrawText("Moar Text Here", 420, 420, 15, RED);
	//draw buttons
	TestPlaygroundDrawButtons();
	//show fps (not needed tbh)
	DrawFPS(10, 10);
    return 0;
}

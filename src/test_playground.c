#include "button.h"
#include "main.h"
#include "test_playground.h"
#include "startup.h"
#include "raylib.h"

//static functions
int  DrawTestPlayground(void);
bool IsTestButtonClickToHideBeingPressed(Vector2 mousePos);
void DrawTestPlayground_ButtonClickToHide(void);
void DrawTestPlaygroundMainMenuButton(void);
bool IsMainMenuButtonBeingPressed(Vector2 mousePos);
void DrawTestPlaygroundExitButton(void);

void TestPlaygroundCheckButtonPress(void);

//variables
bool  buttonMainMenuSizeReady = false;



Button TestPlaygroundButtons[TEST_PLAYGROUND_BUTTON_COUNT];

void TestPlaygroundInitButtons(void)
{
	TraceLog(LOG_INFO, "INIT BUTTONS");
	TestPlaygroundButtons[0] = MakeButton("MAIN", BLUE);
	TestPlaygroundButtons[1] = MakeButton("EXIT", RED);
	TestPlaygroundButtons[2] = MakeButton("HIDE", GREEN);
}

void TestPlaygroundCheckButtonPress(void)
{
    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        return;
	}
	int buttonPress = -1;
	Vector2 mousePos = GetMousePosition();
	for (int i = 0; i < TEST_PLAYGROUND_BUTTON_COUNT; i++) {
		if (CheckCollisionPointRec(mousePos, TestPlaygroundButtons[i].rec)) {
			buttonPress = i;
		}
	}
	if (buttonPress == -1) {
		return;
	}
	switch (buttonPress) {
		//Main
		case 0:
			StartupInitVars();
			mainGameState = MAIN_GAME_STATE_STARTUP;
			break;
		//Exit
		case 1:
			gameRunning = false;
			break;
		//Hide
		case 2:
			TestPlaygroundButtons[2].visible = false;
			break;
		default:
			CloseWindow();
			break;
	}
    return;
}

void TestPlaygroundResizeButtons(void) {
	float screenWidth  = (float)GetScreenWidth();
	float screenHeight = (float)GetScreenHeight();
	for (int i = 0; i < TEST_PLAYGROUND_BUTTON_COUNT; i++) {
		TestPlaygroundButtons[i].rec.width = screenWidth / 10;
		TestPlaygroundButtons[i].rec.height = screenHeight / 10;
		TestPlaygroundButtons[i].rec.y = screenHeight - 2 * (screenHeight / 10);
		//x is tougher
		//0 quarter screen width - half button width
		//1 half screen width - half button width = centered
		//2 three quarder screen width - half button width
		TestPlaygroundButtons[i].rec.x = (screenWidth / 4) + ((screenWidth / 4) * (float)i) - TestPlaygroundButtons[i].rec.width / 2;
	}
	//buttons have been resized for the new window
	buttonMainMenuSizeReady = true;
	TraceLog(LOG_INFO, "BUTTONS RESIZED");
	return;
}

void TestPlaygroundDrawButtons(void) {
	//First handle button resizes
	if (IsWindowResized()) {
		buttonMainMenuSizeReady = false;
	}
	if (!buttonMainMenuSizeReady) {
		TestPlaygroundResizeButtons();
	}
	DrawButtonArray(TestPlaygroundButtons, TEST_PLAYGROUND_BUTTON_COUNT);
}

int DrawTestPlayground(void)
{
	//clear
	ClearBackground(RAYWHITE);
	//draw message text
	DrawText("Handful of Rain", 240, 240, 40, DARKGRAY);
	DrawText("Moar Text Here", 420, 420, 15, RED);
	//draw buttons
	TestPlaygroundDrawButtons();
	//check button press
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		TestPlaygroundCheckButtonPress(); 
	}
	//show fps (not needed tbh)
	DrawFPS(10, 10);
    return 0;
}

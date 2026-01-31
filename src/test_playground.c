#include "button.h"
#include "init.h"
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
bool  buttonsTestPlaygroundReady = false;


Button TestPlaygroundButtons[TEST_PLAYGROUND_BUTTON_COUNT];

void TestPlaygroundInitButtons(void)
{
	TraceLog(LOG_INFO, "INIT BUTTONS");
	buttonsTestPlaygroundReady = false;
	TestPlaygroundButtons[0] = MakeButton("RESET", ORANGE);
	TestPlaygroundButtons[1] = MakeButton("MAIN", BLUE);
	TestPlaygroundButtons[2] = MakeButton("EXIT", RED);
	TestPlaygroundButtons[3] = MakeButton("HIDE", GREEN);
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
		//Reset
		case 0:
			InitStartup();
			buttonsTestPlaygroundReady = false;
			mainGameState = MAIN_GAME_STATE_STARTUP;
			break;
		//Main
		case 1:
			mainGameState = MAIN_GAME_STATE_MAIN_MENU;
			break;
		//Exit
		case 2:
			gameRunning = false;
			break;
		//Hide
		case 3:
			TestPlaygroundButtons[3].visible = false;
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
//		TestPlaygroundButtons[i].rec.x = (screenWidth / 4) + ((screenWidth / 4) * (float)i) - TestPlaygroundButtons[i].rec.width / 2;
		TestPlaygroundButtons[i].rec.x = ((screenWidth / (float)TEST_PLAYGROUND_BUTTON_COUNT) * (float)i) + ((screenWidth - (TestPlaygroundButtons[i].rec.width * (float)TEST_PLAYGROUND_BUTTON_COUNT)) / ((float)TEST_PLAYGROUND_BUTTON_COUNT * 2));
	}
	//buttons have been resized for the new window
	buttonsTestPlaygroundReady = true;
	TraceLog(LOG_INFO, "BUTTONS RESIZED");
	return;
}

void TestPlaygroundDrawButtons(void) {
	//First handle button resizes
	if (IsWindowResized()) {
		buttonsTestPlaygroundReady = false;
	}
	if (!buttonsTestPlaygroundReady) {
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

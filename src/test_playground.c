#include "main.h"
#include "test_playground.h"
#include "raylib.h"

#define TEST_PLAYGROUND_BUTTON_COUNT 3

//static functions
int  DrawTestPlayground(void);
bool IsTestButtonClickToHideBeingPressed(Vector2 mousePos);
void DrawTestPlayground_ButtonClickToHide(void);
void DrawTestPlaygroundMainMenuButton(void);
bool IsMainMenuButtonBeingPressed(Vector2 mousePos);
void DrawTestPlaygroundExitButton(void);

void TestPlaygroundCheckButtonPress(void);

//variables
bool displayButtonClickToHide = true;
bool  buttonMainMenuSizeReady = false;



Button TestPlaygroundButtons[TEST_PLAYGROUND_BUTTON_COUNT];

void TestPlaygroundInitButtons(void)
{
	TestPlaygroundButtons[0] = (Button){ .text = "MAIN", .bg_color = BLUE };
	TestPlaygroundButtons[1] = (Button){ .text = "EXIT", .bg_color = RED };
	TestPlaygroundButtons[2] = (Button){ .text = "HIDE", .bg_color = GREEN };
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
			mainGameState = MAIN_GAME_STATE_STARTUP;
			break;
		//Exit
		case 1:
			CloseWindow();
			break;
		//Hide
		case 2:
			displayButtonClickToHide = false;
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

void DrawButtonTextCentered(const Button *button)
{
	//First set font size to 2/3 the height
	//*I think this is incorrect. You should start with the while loop set font size to 0 then increase until text is too tall or too wide*
	float fontSize;
	Vector2 textSize, textPos;
	fontSize = button->rec.height * 0.6f;
	textSize = MeasureTextEx(GetFontDefault(), button->text, fontSize, 1);
	//Next if the text is too wide scale it downa
	while (textSize.x > button->rec.width - 2) {
		fontSize--;
		textSize = MeasureTextEx(GetFontDefault(), button->text, fontSize, 1);
	}
	//Set X and Y locations for textPos
	textPos.x = button->rec.x  - 1 + (button->rec.width  - textSize.x) / 2;
	textPos.y = button->rec.y + (button->rec.height - textSize.y) / 2;
	DrawTextEx(GetFontDefault(), button->text, textPos, fontSize, 1.0f, BLACK);
}

void TestPlaygroundDrawButtons(void) {
	//First handle button resizes
	if (IsWindowResized()) {
		buttonMainMenuSizeReady = false;
	}
	if (!buttonMainMenuSizeReady) {
		TestPlaygroundResizeButtons();
	}
	for (int i = 0; i < TEST_PLAYGROUND_BUTTON_COUNT; i++) {
		Button button = TestPlaygroundButtons[i];
		if ( i == 2 && !displayButtonClickToHide) {
			continue;
		}
		DrawRectangleRec(button.rec, button.bg_color);
		DrawButtonTextCentered(&button);
	}
	return;
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

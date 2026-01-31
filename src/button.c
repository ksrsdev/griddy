#include "button.h"

void DrawButtonHighlight(const Button *button);

Button MakeButton(const char* text, Color color)
{
	return (Button){
		.rec = {0}, //sizes set dynamically this is just init
		.text = text,
		.bg_color = color,
		.visible = true,
        .highlight = false
	};
}

void DrawButton(const Button *button)
{
	if (!button->visible) {
		return;
	}
    if (button->highlight) {
        DrawButtonHighlight(button);
    }
	DrawRectangleRec(button->rec, button->bg_color);
	DrawButtonTextCentered(button);
}

void DrawButtonHighlight(const Button *button)
{
    Rectangle highlight;
    //Same margin width on both sides
    if (button->rec.width >= button->rec.height) {
        //use width
        highlight.width = button->rec.width + button->rec.width / 20;
        highlight.height = button->rec.height + button->rec.width / 20;
        highlight.x = button->rec.x - button->rec.width / 40;
        highlight.y = button->rec.y - button->rec.width / 40;
    } else {
        //use height
        highlight.height = button->rec.height + button->rec.height / 20;
        highlight.width = button->rec.width + button->rec.height / 20;
        highlight.y = button->rec.y - button->rec.height / 40;
        highlight.x = button->rec.x - button->rec.height / 40;
    }
	if (ColorsEqual(button->bg_color, BLACK)) {
		DrawRectangleRec(highlight, RED);
	} else {
		DrawRectangleRec(highlight, BLACK);
	}
}

bool ColorsEqual(Color col1, Color col2) {
    return (col1.r == col2.r && 
            col1.g == col2.g && 
            col1.b == col2.b && 
            col1.a == col2.a);
}

void DrawButtonTextCentered(const Button *button)
{
	//*I think this is incorrect. You should start with the while loop set font size to 0 then increase until text is too tall or too wide*
	float fontSize;
	Vector2 textSize, textPos;
	fontSize = button->rec.height * 0.6f;
	textSize = MeasureTextEx(GetFontDefault(), button->text, fontSize, 1);
	//Next if the text is too wide scale it down
	while (textSize.x > button->rec.width - 2) {
		fontSize--;
		textSize = MeasureTextEx(GetFontDefault(), button->text, fontSize, 1);
	}
	//Set X and Y locations for textPos
	textPos.x = button->rec.x  - 1 + (button->rec.width  - textSize.x) / 2;
	textPos.y = button->rec.y + (button->rec.height - textSize.y) / 2;
	if (ColorsEqual(button->bg_color, BLACK)) {
		DrawTextEx(GetFontDefault(), button->text, textPos, fontSize, 1.0f, WHITE);
	} else {
		DrawTextEx(GetFontDefault(), button->text, textPos, fontSize, 1.0f, BLACK);
	}
}

void DrawButtonArray(const Button *buttonArray, int arraySize)
{
	for (int i = 0; i < arraySize; i++) {
		Button button = buttonArray[i];
        Vector2 mousePos = GetMousePosition();
        if (CheckCollisionPointRec(mousePos, button.rec)) {
            button.highlight = true;
        } else {
            button.highlight = false;
        }
		DrawButton(&button);
	}
}

void DrawSingleButton(Button *button)
{
	Vector2 mousePos = GetMousePosition();
	if (CheckCollisionPointRec(mousePos, button->rec)) {
		button->highlight = true;
	} else {
		button->highlight = false;
	}
	DrawButton(button);
}

void RepositionSingleButton_BottomLeft(Button *button)
{
	//margins given as percentages of the screen
	//This one is 10, 10
	float marginX = 15;
	float marginY = 15;
	float screenWidth = (float)GetScreenWidth();
	float screenHeight = (float)GetScreenHeight();
	button->rec.x = marginX;
	button->rec.width = screenWidth / 15;
	button->rec.height = screenHeight / 15;
	button->rec.y = screenHeight - marginY - button->rec.height;
}

void RepositionSingleButton_BottomRight(Button *button)
{
	//margins given as percentages of the screen
	//This one is 10, 10
	float marginX = 15;
	float marginY = 15;
	float screenWidth = (float)GetScreenWidth();
	float screenHeight = (float)GetScreenHeight();
	button->rec.width = screenWidth / 8;
	button->rec.x = screenWidth - marginX - button->rec.width;
	button->rec.height = screenHeight / 15;
	button->rec.y = screenHeight - marginY - button->rec.height;
}

void RepositionButtonArray_CenteredVertical(Button *buttonArray, const int arraySize, float marginX, float marginY)
{
	//margins given as percentages of the screen
	float screenWidth = (float)GetScreenWidth();
	float screenHeight = (float)GetScreenHeight();
	marginX = screenWidth * (marginX / 100);
	marginY = screenHeight * (marginY / 100);
	for (int i=0; i<arraySize; i++) {
		buttonArray[i].rec.width = screenWidth - (2 * marginX);
		buttonArray[i].rec.height = (screenHeight - (2 * marginY)) / (((float)arraySize * 2) - 1);
		buttonArray[i].rec.x = marginX;
		buttonArray[i].rec.y = marginY + (buttonArray[i].rec.height * ((float)i * 2));
	}
}

//Horizontal Button Array at variable Height
void RepositionButtonArray_VariableHorizontal(Button *buttonArray, const int arraySize, float marginX, float rowHeight, float rowY)
{
	float screenWidth = (float)GetScreenWidth();
	float screenHeight = (float)GetScreenHeight();
	marginX = screenWidth * (marginX / 100);
	rowHeight = screenHeight * (rowHeight / 100);
	float screenRatio = screenWidth / screenHeight;
	rowY = screenHeight * (rowY / 100);
	for (int i=0; i< arraySize; i++) {
		buttonArray[i].rec.height = rowHeight;
		buttonArray[i].rec.y = rowY - (rowHeight / 2);
		buttonArray[i].rec.width = rowHeight * screenRatio;
		buttonArray[i].rec.x = ((screenWidth / (float)arraySize) * (float)i) + ((screenWidth - (buttonArray[i].rec.width * (float)arraySize)) / ((float)arraySize * 2));
	}
}

//Horizontal Button Array at fixed, centered height
void RepositionButtonArray_CenteredHorizontal(Button *buttonArray, const int arraySize, float marginX, float rowHeight)
{
	//marginX is the x margin, respected on both sides
	//rowHeight is the maximum height of the button row
	//both given as percentages of the screen
	float screenWidth = (float)GetScreenWidth();
	float screenHeight = (float)GetScreenHeight();
	float screenRatio = screenWidth / screenHeight;
	marginX = screenWidth * (marginX / 100);
	rowHeight = screenHeight * (rowHeight / 100);
	for (int i=0; i< arraySize; i++) {
		buttonArray[i].rec.height = rowHeight;
		buttonArray[i].rec.y = (screenHeight / 2) - (rowHeight / 2);
		buttonArray[i].rec.width = buttonArray[i].rec.y * screenRatio;
		buttonArray[i].rec.x = ((screenWidth / (float)arraySize) * (float)i) + ((screenWidth - (buttonArray[i].rec.width * (float)arraySize)) / ((float)arraySize * 2));

	}
}

bool CheckSingleButtonForButtonPress(const Button *button)
{
	Vector2 mousePos = GetMousePosition();
	if (CheckCollisionPointRec(mousePos, button->rec) && button->visible) {
		return true;
	} else {
		return false;
	}
}

int CheckButtonArrayForButtonPress(const Button *buttonArray, const int arraySize)
{
	for (int i=0; i<arraySize; i++) {
		if (CheckSingleButtonForButtonPress(&buttonArray[i])) {
			return i;
		}
	}
	return -1;
}


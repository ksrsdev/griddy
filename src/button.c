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

void ResizeButtonArray(Button *buttonArray, int arraySize, int size)
{
	float screenWidth = (float)GetScreenWidth();
	float screenHeight = (float)GetScreenHeight();
	//size is passed as a percentage of the screenWidth and Height
	for (int i=0; i<arraySize; i++) {
		buttonArray[i].rec.width = screenWidth / (float)size;
		buttonArray[i].rec.height = screenHeight / (float)size;
	}
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
        highlight.width = button->rec.width + button->rec.width / 10;
        highlight.height = button->rec.height + button->rec.width / 10;
        highlight.x = button->rec.x - button->rec.width / 20;
        highlight.y = button->rec.y - button->rec.width / 20;
    } else {
        //use height
        highlight.height = button->rec.height + button->rec.height / 10;
        highlight.width = button->rec.width + button->rec.height / 10;
        highlight.y = button->rec.y - button->rec.height / 20;
        highlight.x = button->rec.x - button->rec.height / 20;
    }
    DrawRectangleRec(highlight, BLACK);
}

void DrawButtonTextCentered(const Button *button)
{
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

void DrawButtonArray(const Button *buttonArray, int arraySize)
{
	TraceLog(LOG_INFO, "DrawButtonArray()");
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


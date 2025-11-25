#include "button.h"

Button MakeButton(const char* text, Color color)
{
	return (Button){
		.rec = {0}, //sizes set dynamically this is just init
		.text = text,
		.bg_color = color,
		.visible = true
	};
}

void DrawButton(const Button *button)
{
	if (!button->visible) {
		return;
	}
	DrawRectangleRec(button->rec, button->bg_color);
	DrawButtonTextCentered(button);
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
	for (int i = 0; i < arraySize; i++) {
		Button button = buttonArray[i];
		DrawButton(&button);
	}
}


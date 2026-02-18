#ifndef GUARD_BUTTON_H
#define GUARD_BUTTON_H

#include "raylib.h"

typedef struct {
	Rectangle rec;
	const char *text;
	Color bg_color;
	bool visible;
    bool highlight;
} Button;

Button MakeButton(const char* text, Color color);

void DrawButton(const Button *button);
void DrawSingleButton(Button *button);
void DrawButtonArray(const Button *buttonArray, int arraySize);
void DrawButtonTextCentered(const Button *button);
void RepositionSingleButton_BottomLeft(Button *button);
void RepositionSingleButton_BottomRight(Button *button);
void RepositionButtonArray_CenteredVertical(Button *buttonArray, const int arraySize, float marginX, float marginY);
void RepositionButtonArray_CenteredHorizontal(Button *buttonArray, const int arraySize, float marginX, float marginY);
void RepositionButtonArray_VariableHorizontal(Button *buttonArray, const int arraySize, float marginX, float rowHeight, float rowY);
bool CheckSingleButtonForButtonPress(const Button *button);
int CheckButtonArrayForButtonPress(const Button *buttonArray, const int arraySize);

#endif

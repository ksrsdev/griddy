#include "raylib.h"

typedef struct {
	Rectangle rec;
	const char* text;
	Color bg_color;
	bool visible;
    bool highlight;
} Button;

Button MakeButton(const char* text, Color color);

void DrawButton(const Button *button);
void DrawButtonArray(const Button *buttonArray, int arraySize);
void DrawButtonTextCentered(const Button *button);
void RepositionButtonArray_CenteredVertical(Button *buttonArray, const int arraySize, float marginX, float marginY);
int CheckButtonArrayForButtonPress(const Button *buttonArray, const int arraySize);

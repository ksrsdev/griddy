#include "raylib.h"

int DrawTestPlayground(void);
void TestPlaygroundInitButtons(void);
void TestPlaygroundResizeButtons(void);
void TestPlaygroundDrawButtons(void);

typedef struct {
	Rectangle rec;
	const char* text;
	Color bg_color;
} Button;

void DrawButtonTextCentered(const Button *button);

#define TEST_PLAYGROUND_BUTTON_COUNT 3


#ifndef GUARD_OPTIONS_MENU_H
#define GUARD_OPTIONS_MENU_H

#include "button.h"
#include "text.h"

#define OPTIONS_MENU_BUTTON_COUNT 3

typedef struct {
	TextBox title;
	Button backButton;
	Button menuButtons[OPTIONS_MENU_BUTTON_COUNT];
} OptionsMenuLayout;

void DrawOptionsMenu(void);
void InitOptionsMenuButtons(void);

#endif

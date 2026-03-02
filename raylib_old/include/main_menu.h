#ifndef GUARD_MAIN_MENU_H
#define GUARD_MAIN_MENU_H

#include "button.h"
#include "text.h"

#define MAIN_MENU_BUTTON_COUNT 6

typedef struct {
	TextBox title;
	TextBox splash;
	TextBox version;
	Button menuButtons[MAIN_MENU_BUTTON_COUNT];
} MainMenuLayout;

void DrawMainMenu(void);
void InitMainMenuButtons(void);


#endif

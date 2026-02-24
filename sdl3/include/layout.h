#ifndef GUARD_LAYOUT_H
#define GUARD_LAYOUT_H

//TODO: Remove after SDL_FRect test rect removed from layout struct
#include <SDL3/SDL.h>


#include "button.h"
#include "text.h"

typedef struct{
	TextBox title;
	//TODO: Remove SDL Include when you remove this
	SDL_FRect rect;
	uint64_t startTime;
} IntroLayout;

#define MAIN_MENU_BUTTON_COUNT 6

typedef struct {
	TextBox title;
	TextBox splash;
	TextBox version;
	Button menuButtons[MAIN_MENU_BUTTON_COUNT];
} MainMenuLayout;

#undef MAIN_MENU_BUTTON_COUNT

#endif

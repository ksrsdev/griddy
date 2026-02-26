#ifndef GUARD_LAYOUT_H
#define GUARD_LAYOUT_H

#include <stdbool.h>

//TODO: Remove after SDL_FRect test rect removed from layout struct
#include <SDL3/SDL.h>


#include "button.h"
#include "text.h"

typedef enum {
	INTRO_STEP_NONE,
	INTRO_STEP_INTRO,         //Text fades in
	INTRO_STEP_TRANSITION,    //Text colors flip
	INTRO_STEP_HOLD,          //Text holds flipped colors
	INTRO_STEP_COUNT
} IntroStep;

typedef struct {
	//TODO: Remove SDL Include when you remove this
	SDL_FRect titleDestRec;
	uint64_t startTime;
	bool introComplete;
	IntroStep introStep;
	SDL_Color backgroundColor;
	SDL_Color textColor;
} IntroData;

#define MAIN_MENU_BUTTON_COUNT 6

typedef struct {
	TextBox title;
	TextBox splash;
	TextBox version;
	Button menuButtons[MAIN_MENU_BUTTON_COUNT];
} MainMenuData;

#undef MAIN_MENU_BUTTON_COUNT

#endif

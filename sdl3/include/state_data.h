#ifndef GUARD_LAYOUT_H
#define GUARD_LAYOUT_H

#include <stdbool.h>

#include <SDL3/SDL.h>

#include "button.h"
#include "text.h"

typedef enum {
	INTRO_STEP_NONE,
	INTRO_STEP_ANIM,       //Text fades in
	INTRO_STEP_TRANSITION, //Text colors flip
	INTRO_STEP_HOLD,       //Text holds flipped colors
	INTRO_STEP_COUNT
} IntroStep;

typedef enum {
	INTRO_ANIM_NONE,
	INTRO_ANIM_ZOOM,
	INTRO_ANIM_SLIDE_NORTH,
	INTRO_ANIM_SLIDE_SOUTH,
	INTRO_ANIM_SLIDE_EAST,
	INTRO_ANIM_SLIDE_WEST,
	INTRO_ANIM_SWIRL,
	INTRO_ANIM_LOOPS,
	INTRO_ANIM_COUNT
} IntroAnim;

typedef struct {
	SDL_FRect titleDestRect; //Dest for titleTargetTexture
	uint64_t startTime;
	double textureRotation;
	IntroStep introStep;
	IntroAnim introAnim;
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

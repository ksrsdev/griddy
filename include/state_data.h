#ifndef GUARD_LAYOUT_H
#define GUARD_LAYOUT_H

#include <stdbool.h>

#include <SDL3/SDL.h>

#include "error.h"
#include "text.h"
#include "ui.h"

//ERROR
typedef struct {
	UIData uiData[ERROR_UI_COUNT];
	const char *uiStrings[ERROR_UI_COUNT];
	bool texturesNeedResizing;
} ErrorData;

//INTRO
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
	INTRO_ANIM_LOOP,
	INTRO_ANIM_COUNT
} IntroAnim;

typedef struct {
	UIData titleData;
	uint64_t startTime;
	IntroStep introStep;
	IntroAnim introAnim;
} IntroData;

#endif

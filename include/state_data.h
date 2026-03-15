#ifndef GUARD_LAYOUT_H
#define GUARD_LAYOUT_H

#include <stdbool.h>

#include <SDL3/SDL.h>

#include "button.h"
#include "text.h"
#include "error.h"

typedef enum {
	UI_TYPE_NONE,
	UI_TYPE_TEXT,
	UI_TYPE_BUTTON,
	UI_TYPE_COUNT
} UIType;

typedef struct {
	SDL_FRect destRect;     //16 bytes
	OnClick onClick;        //8  bytes
	f64 rotation;           //8  bytes
	SDL_Color fg;           //4  bytes
	SDL_Color bg;           //4  bytes
	SDL_Color outlineColor; //4  bytes
	u32 type;               //4  bytes
	bool hidden;            //1  bytes
	bool outlined;          //1  bytes
	bool isHovered;         //1  bytes
	bool hasBackground;     //1  bytes
	//Extra space before 64 byte threshold
	u8 reserved[12];        //12 bytes
} UIData;

//ERROR
typedef struct {
	UIData uiData[ERROR_UI_COUNT];
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
	SDL_FRect titleDestRect; //Dest for titleTargetTexture
	uint64_t startTime;
	double textureRotation;
	IntroStep introStep;
	IntroAnim introAnim;
} IntroData;

#endif

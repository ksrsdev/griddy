#include "intro.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_shadercross/SDL_shadercross.h>

#include "colors.h"
#include "context.h"
#include "error.h"
#include "error_code.h"
#include "init.h"
#include "render.h"
#include "state_data.h"
#include "types.h"
#include "update.h"

#define INTRO_ANIM_TIME 1000.00f
#define INTRO_HOLD_TIME 750.00f

static void NoneAnim(IntroData *introData, const Vector2 windowSize, const u64 deltaTime);
static void ZoomAnim(IntroData *introData, const Vector2 windowSize, const u64 deltaTime);
static void SlideNorthAnim(IntroData *introData, const Vector2 windowSize, const u64 deltaTime);
static void SlideSouthAnim(IntroData *introData, const Vector2 windowSize, const u64 deltaTime);
static void SlideEastAnim(IntroData *introData, const Vector2 windowSize, const u64 deltaTime);
static void SlideWestAnim(IntroData *introData, const Vector2 windowSize, const u64 deltaTime);
static void SlideAnim(IntroData *introData, const Vector2 windowSize, const u64 deltaTime, Direction dir);
static void SlideAnimVertical(IntroData *introData, const float wX, const float wY, const u64 deltaTime, Direction dir);
static void SlideAnimHorizontal(IntroData *introData, const float wX, const float wY, const u64 deltaTime, Direction dir);
static void SwirlAnim(IntroData *introData, const Vector2 windowSize, const u64 deltaTime);
static void LoopAnim(IntroData *introData, const Vector2 windowSize, const u64 deltaTime);

static void ScaleTextureDestRectForAnim(IntroData *introData, const Vector2 windowSize, const u64 deltaTime);
static void UpdateStepAfterAnim(IntroStep *introStep);

static const IntroAnimFunc IntroAnimTable[] = {
	[INTRO_ANIM_NONE]        = NoneAnim,
	[INTRO_ANIM_ZOOM]        = ZoomAnim,
	[INTRO_ANIM_SLIDE_NORTH] = SlideNorthAnim,
	[INTRO_ANIM_SLIDE_SOUTH] = SlideSouthAnim,
	[INTRO_ANIM_SLIDE_EAST]  = SlideEastAnim,
	[INTRO_ANIM_SLIDE_WEST]  = SlideWestAnim,
	[INTRO_ANIM_SWIRL]       = SwirlAnim,
	[INTRO_ANIM_LOOP]        = LoopAnim,
};

void Intro_Init(GameEngine *eng, GameData *data)
{
	//Create intro state data
	data->stateData = calloc(1, sizeof(IntroData));
	if (data->stateData == NULL) {
		//ERROR!
		return;
	}
	
	//local pointers
	IntroData *introData = data->stateData;

	//#UI
	introData->titleData.type = UI_TYPE_TEXT;
	introData->titleData.fg = COLOR_WHITE;
	SDL_FRect initRect = {1, 1, 1, 1};
	introData->titleData.destRect = initRect;
	SDL_Texture *text = Text_CreateUITexture(eng, "GRIDDY", &introData->titleData);

	introData->titleData.texture = text;

	//#State vars
	
	//Anim start time
	introData->startTime = SDL_GetTicks();

	//Test intro anim
//	introData->introAnim = INTRO_ANIM_SWIRL;

	//Random intro anim
	introData->introAnim = (IntroAnim)(rand() % (INTRO_ANIM_COUNT - INTRO_ANIM_ZOOM)) + INTRO_ANIM_ZOOM;

}

//This should be a "mirror" of the init function - top bottom -> bottom top
void Intro_Cleanup(GameEngine *eng, GameData *data)
{
	(void)eng;
	IntroData *introData = data->stateData;
	

	if (introData->titleData.texture != NULL) {
		SDL_DestroyTexture(introData->titleData.texture);
	}

	Deinit_StateData(&data->stateData);
}

void Intro_Update(GameData *data)
{

	//local stateData pointer
	IntroData *introData = data->stateData;

	u64 currentTime = SDL_GetTicks();
	u64 deltaTime = currentTime - introData->startTime;

	//Handle Intro Anim
	if (introData->introAnim > INTRO_ANIM_NONE && introData->introAnim < INTRO_ANIM_COUNT) {
		IntroAnimFunc animFunc = IntroAnimTable[introData->introAnim];
		if (animFunc) {
			animFunc(introData, data->window.size, deltaTime);
		}
	} else {
		//ERROR
		return;
	}

	//TODO: Handle state transition to main menu when times up
	if (deltaTime > INTRO_ANIM_TIME + INTRO_HOLD_TIME) {
		RequestGameStateTransition(data, GAME_STATE_MAIN_MENU);
		//Error_Alert(data, ERROR_ALLOC, "Hello this is some sample text for testing some stuffs :D");
	}

}

void Intro_Render(const GameEngine *eng, const GameData *data)
{
	IntroData *introData = data->stateData;
	SDL_Color bgColor = {0};
	
	switch (introData->introStep) {
		case INTRO_STEP_ANIM:
			bgColor = COLOR_BLACK;
			break;
		case INTRO_STEP_TRANSITION:
			bgColor = COLOR_WHITE;
			introData->titleData.fg = COLOR_BLACK;
			break;
		case INTRO_STEP_HOLD:
			bgColor = COLOR_WHITE;
			break;
		default:
			//ERROR
			break;
	}

	//Background
	Render_SetDrawColor(eng->renderer, bgColor);
	SDL_RenderClear(eng->renderer);

	UI_RenderUIElement(eng, &introData->titleData);
}

// ###   STATIC FUNCS   ###

static void NoneAnim(IntroData *introData, const Vector2 windowSize, const u64 deltaTime) 
{
	(void)introData;
	(void)windowSize;
	(void)deltaTime;
}

#define TITLE_ASPECT_RATIO 1.50

static void ZoomAnim(IntroData *introData, const Vector2 windowSize, const u64 deltaTime) 
{

	ScaleTextureDestRectForAnim(introData, windowSize, deltaTime);
	
	//center rect
	introData->titleData.destRect.x = ((float)windowSize.x / 2.0f) - (introData->titleData.destRect.w / 2.0f);
	introData->titleData.destRect.y = ((float)windowSize.y / 2.0f) - (introData->titleData.destRect.h / 2.0f);

}

static void SlideNorthAnim(IntroData *introData, const Vector2 windowSize, const u64 deltaTime) 
{
	SlideAnim(introData, windowSize, deltaTime, DIR_NORTH);
}

static void SlideSouthAnim(IntroData *introData, const Vector2 windowSize, const u64 deltaTime) 
{
	SlideAnim(introData, windowSize, deltaTime, DIR_SOUTH);
}

static void SlideEastAnim(IntroData *introData, const Vector2 windowSize, const u64 deltaTime)
{
	SlideAnim(introData, windowSize, deltaTime, DIR_EAST);
}

static void SlideWestAnim(IntroData *introData, const Vector2 windowSize, const u64 deltaTime)
{
	SlideAnim(introData, windowSize, deltaTime, DIR_WEST);
}

static void SlideAnim(IntroData *introData, const Vector2 windowSize, const u64 deltaTime, Direction dir)
{
	float wX = (float)windowSize.x;
	float wY = (float)windowSize.y;
	
	introData->titleData.destRect.w = wX * 0.6f;
	introData->titleData.destRect.h = wY * 0.6f;

	//Vertical or Horizontal:
	if (dir == DIR_NORTH || dir == DIR_SOUTH) {
		SlideAnimVertical(introData, wX, wY, deltaTime, dir);
	} else if (dir == DIR_EAST || dir == DIR_WEST) {
		SlideAnimHorizontal(introData, wX, wY, deltaTime, dir);
	} else {
		//ERROR dir OOB
		return;
	}
}

static void SlideAnimVertical(IntroData *introData, const float wX, const float wY, const u64 deltaTime, Direction dir)
{
	//Position Rect centered horizontally
	introData->titleData.destRect.x = (wX / 2.0f) - (introData->titleData.destRect.w / 2.0f);

	float offset = (wY / 2.0f) - introData->titleData.destRect.h / 2.0f;

	if (deltaTime <= INTRO_ANIM_TIME) {
		float offsetMod = ((INTRO_ANIM_TIME - (float)deltaTime) * (wY / 2000.0f));
		introData->introStep = INTRO_STEP_ANIM;
		if (dir == DIR_NORTH) {
			offset += offsetMod;
		} else if (dir == DIR_SOUTH) {
			offset -= offsetMod;
		} else {
			//ERROR dir OOB
			return;
		}
	} else {
		UpdateStepAfterAnim(&introData->introStep);
	}

	introData->titleData.destRect.y = offset;
}

static void SlideAnimHorizontal(IntroData *introData, const float wX, const float wY, const u64 deltaTime, Direction dir)
{
	//Center rect vertically
	introData->titleData.destRect.y = (wY / 2.0f) - (introData->titleData.destRect.h / 2.0f);

	float offset = (wX / 2.0f) - (introData->titleData.destRect.w / 2.0f);

	if (deltaTime <= INTRO_ANIM_TIME) {
		float offsetMod = ((INTRO_ANIM_TIME - (float)deltaTime) * (wX / 2000.0f));
		introData->introStep = INTRO_STEP_ANIM;
		if (dir == DIR_EAST) {
			offset -= offsetMod;
		} else if (dir == DIR_WEST) {
			offset += offsetMod;
		} else {
			//ERROR dir OOB
			return;
		}
	} else {
		UpdateStepAfterAnim(&introData->introStep);
	}

	introData->titleData.destRect.x = offset;
}

#define NUM_SWIRL_ROTATIONS 4
#define ROTATION_ANGLE 360
static void SwirlAnim(IntroData *introData, const Vector2 windowSize, const u64 deltaTime)
{
	ScaleTextureDestRectForAnim(introData, windowSize, deltaTime);

	float angle = ((float)deltaTime / INTRO_ANIM_TIME) * (NUM_SWIRL_ROTATIONS * ROTATION_ANGLE);
	if (introData->introStep != INTRO_STEP_HOLD) {
		introData->titleData.rotation = (double)angle;
	} else {
		introData->titleData.rotation = 0;
	}
	
	//center rect
	introData->titleData.destRect.x = ((float)windowSize.x / 2.0f) - (introData->titleData.destRect.w / 2.0f);
	introData->titleData.destRect.y = ((float)windowSize.y / 2.0f) - (introData->titleData.destRect.h / 2.0f);
}

static void LoopAnim(IntroData *introData, const Vector2 windowSize, const u64 deltaTime)
{
	ScaleTextureDestRectForAnim(introData, windowSize, deltaTime);

	//Loop angle and radius
	float angle = (float)deltaTime / 90.0f;
	float maxRadius = 0;

	//radius constrained by X or Y
	if (windowSize.x > windowSize.y) {
		maxRadius = (((float)windowSize.y / 2.0f)) - (introData->titleData.destRect.h / 2.0f);
	} else {
		maxRadius = (((float)windowSize.x / 2.0f)) - (introData->titleData.destRect.w / 2.0f);
	}

	//scale radius
	float radius = maxRadius * ( (INTRO_ANIM_TIME - (float)deltaTime) / INTRO_ANIM_TIME);
	if (introData->introStep != INTRO_STEP_ANIM) {
		radius = 0;
	}

	//center to screen center + radius @ theta
	introData->titleData.destRect.x = ((float)windowSize.x / 2.0f) + ((float)cos(angle) * radius);
	introData->titleData.destRect.y = ((float)windowSize.y / 2.0f) + ((float)sin(angle) * radius);

	//Offset XY since texture xy is top left corner
	introData->titleData.destRect.x -= introData->titleData.destRect.w / 2.0f;
	introData->titleData.destRect.y -= introData->titleData.destRect.h / 2.0f;
}

#undef NUM_SWIRL_ROTATIONS 
#undef ROTATION_ANGLE

static void ScaleTextureDestRectForAnim(IntroData *introData, const Vector2 windowSize, const u64 deltaTime)
{
	float scale = 0;
	float wX = (float)windowSize.x;
	float wY = (float)windowSize.y;
	
	//Should we use deltaTime for scale or is it already 'scaled' enough
	if (deltaTime < INTRO_ANIM_TIME) {
		scale = (float)deltaTime / INTRO_ANIM_TIME;
		introData->introStep = INTRO_STEP_ANIM;
	} else {
		scale = 1.0;
		UpdateStepAfterAnim(&introData->introStep);
	}

	//Assign data to destRect
	introData->titleData.destRect.w = wX * scale * 0.6f; 
	introData->titleData.destRect.h = wY * scale * 0.6f; 
}

#undef INTRO_ANIM_TIME
#undef INTRO_HOLD_TIME
#undef TITLE_ASPECT_RATIO

static void UpdateStepAfterAnim(IntroStep *introStep)
{
	if (*introStep == INTRO_STEP_ANIM) {
		*introStep = INTRO_STEP_TRANSITION;
	} else {
		*introStep = INTRO_STEP_HOLD;
	}
}



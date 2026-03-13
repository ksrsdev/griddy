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
#include "render.h"
#include "state_data.h"
#include "state_resources.h"
#include "types.h"

#define INTRO_ANIM_TIME 1000.00f
#define INTRO_HOLD_TIME  500.00f

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
static void EnforceTitleAspectRatio(SDL_FRect *rect, const float wX, const float aspectRatio);
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

	//Create intro resources
	eng->stateResources = calloc(1, sizeof(IntroResources));
	if (eng->stateResources == NULL) {
		//ERROR!
		return;
	}

	//Create intro state data
	data->stateData = calloc(1, sizeof(IntroData));
	if (data->stateData == NULL) {
		//ERROR!
		return;
	}
	
	//local pointers
	IntroData *introData = data->stateData;
	IntroResources *introResources = eng->stateResources;

	//Load Resources
	introResources->title = TTF_CreateText(eng->textEngine, eng->font, "GRIDDY", 0);
	Text_SetColor(introResources->title, COLOR_WHITE);
	//introResources->titleTargetTexture = CreateTextureViaSurfaceFromText(eng->renderer, eng->fonts.title, "GRIDDY");
	introResources->titleTargetTexture = CreateTextureFromText(eng->renderer, introResources->title);
	SDL_SetTextureBlendMode(introResources->titleTargetTexture, SDL_BLENDMODE_BLEND);
	SDL_SetTextureScaleMode(introResources->titleTargetTexture, SDL_SCALEMODE_LINEAR);
//	SDL_SetTextureAddressMode(introResources->titleTargetTexture, SDL_TEXTUREADDRESS_CLAMP);

	//Load Data
	introData->startTime = SDL_GetTicks();

	//Test intro anim
	introData->introAnim = INTRO_ANIM_ZOOM;

	//Random intro anim
//	introData->introAnim = (IntroAnim)(rand() % (INTRO_ANIM_COUNT - INTRO_ANIM_ZOOM)) + INTRO_ANIM_ZOOM;

}

//This should be a "mirror" of the init function - top bottom -> bottom top
void Intro_Cleanup(GameEngine *eng, GameData *data)
{
	(void)data;
	//Make sure you clean up all your memory. Double check this stuff

	//Local pointers
//	IntroData *introData = data->stateData;
	IntroResources *introResources = eng->stateResources;
	
	//Cleanup Data - Nothing to do rn. 
	
	//Cleanup Resources
	TTF_DestroyText(introResources->title);
	SDL_DestroyTexture(introResources->titleTargetTexture);

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
		Error_Alert(data, ERROR_ALLOC, "Test Error");
	}

}

void Intro_Render(const GameEngine *eng, const GameData *data)
{
	IntroData *introData = data->stateData;
	IntroResources *introResources = eng->stateResources;
	SDL_Color bgColor = {0};
	
	switch (introData->introStep) {
		case INTRO_STEP_ANIM:
			bgColor = COLOR_BLACK;
			break;
		case INTRO_STEP_TRANSITION:
			bgColor = COLOR_WHITE;
			//Re-draw texture in fg Color
			SDL_Color fgColor = COLOR_BLACK;
			Text_SetColor(introResources->title, fgColor);
			break;
		case INTRO_STEP_HOLD:
			bgColor = COLOR_GREEN;
			break;
		default:
			//ERROR
			break;
	}

	//Background
	Render_SetDrawColor(eng->renderer, bgColor);
	SDL_RenderClear(eng->renderer);

	//TODO: Move this into a Render_SetupTextRenderState(eng, textObject, targetTexture)
	//      > It should switch the state, and ensure the color is correct then return void(?)
	//Test shader
	SDL_SetGPURenderState(eng->renderer, eng->sdfRenderState);

	//Get color for text to be rendered by the shader
	u8 r, g, b, a;
	if (!TTF_GetTextColor(introResources->title, &r, &g, &b, &a)) {
		printf("Can't find text color!");
		r = 255;
		g = 255;
		b = 255;
		a = 255;
	} 
	SDL_SetTextureColorMod(introResources->titleTargetTexture, r, g, b);
	SDL_SetTextureAlphaMod(introResources->titleTargetTexture, 255);

	//text
	if (introData->textureRotation == 0 || introData->introStep != INTRO_STEP_ANIM) {
		SDL_RenderTexture (eng->renderer, introResources->titleTargetTexture, NULL, &introData->titleDestRect);
	} else {
		SDL_RenderTextureRotated (eng->renderer, introResources->titleTargetTexture, NULL, &introData->titleDestRect, introData->textureRotation, NULL, SDL_FLIP_NONE);
	}

	//Reset test shader
	SDL_SetGPURenderState(eng->renderer, NULL);

}

// STATIC FUNCS

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
	introData->titleDestRect.x = ((float)windowSize.x / 2.0f) - (introData->titleDestRect.w / 2.0f);
	introData->titleDestRect.y = ((float)windowSize.y / 2.0f) - (introData->titleDestRect.h / 2.0f);

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

	introData->titleDestRect.w = wX / 2.0f;
	introData->titleDestRect.h = wY / 2.0f;

	//Enforce Aspect ratio
	EnforceTitleAspectRatio(&introData->titleDestRect, wX / 2.0f, TITLE_ASPECT_RATIO);

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
	introData->titleDestRect.x = (wX / 2.0f) - (introData->titleDestRect.w / 2.0f);

	float offset = (wY / 2.0f) - introData->titleDestRect.h / 2.0f;

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

	introData->titleDestRect.y = offset;
}

static void SlideAnimHorizontal(IntroData *introData, const float wX, const float wY, const u64 deltaTime, Direction dir)
{
	//Center rect vertically
	introData->titleDestRect.y = (wY / 2.0f) - (introData->titleDestRect.h / 2.0f);

	float offset = (wX / 2.0f) - (introData->titleDestRect.w / 2.0f);

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

	introData->titleDestRect.x = offset;
}

#define NUM_SWIRL_ROTATIONS 5
#define ROTATION_ANGLE 360
static void SwirlAnim(IntroData *introData, const Vector2 windowSize, const u64 deltaTime)
{
	ScaleTextureDestRectForAnim(introData, windowSize, deltaTime);

	float angle = ((float)deltaTime / INTRO_ANIM_TIME) * (NUM_SWIRL_ROTATIONS * ROTATION_ANGLE);
	introData->textureRotation = (double)angle;
	
	//center rect
	introData->titleDestRect.x = ((float)windowSize.x / 2.0f) - (introData->titleDestRect.w / 2.0f);
	introData->titleDestRect.y = ((float)windowSize.y / 2.0f) - (introData->titleDestRect.h / 2.0f);
}

static void LoopAnim(IntroData *introData, const Vector2 windowSize, const u64 deltaTime)
{
	ScaleTextureDestRectForAnim(introData, windowSize, deltaTime);

	//Loop angle and radius
	float angle = (float)deltaTime / 90.0f;
	float maxRadius = 0;

	//radius constrained by X or Y
	if (windowSize.x > windowSize.y) {
		maxRadius = (((float)windowSize.y / 2.0f)) - (introData->titleDestRect.h / 2.0f);
	} else {
		maxRadius = (((float)windowSize.x / 2.0f)) - (introData->titleDestRect.w / 2.0f);
	}

	//scale radius
	float radius = maxRadius * ( (INTRO_ANIM_TIME - (float)deltaTime) / INTRO_ANIM_TIME);
	if (introData->introStep != INTRO_STEP_ANIM) {
		radius = 0;
	}

	//center to screen center + radius @ theta
	introData->titleDestRect.x = ((float)windowSize.x / 2.0f) + ((float)cos(angle) * radius);
	introData->titleDestRect.y = ((float)windowSize.y / 2.0f) + ((float)sin(angle) * radius);

	//Offset XY since texture xy is top left corner
	introData->titleDestRect.x -= introData->titleDestRect.w / 2.0f;
	introData->titleDestRect.y -= introData->titleDestRect.h / 2.0f;
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
	introData->titleDestRect.w = wX * scale / 2.0f;
	introData->titleDestRect.h = wY * scale / 2.0f;
	
	//Ensure rectangle is wider than it is tall to accomodate 6 letter string - 6:4 = 3:2 rect
	float maxWidth = wX * scale / 2.0f;
	EnforceTitleAspectRatio(&introData->titleDestRect, maxWidth, TITLE_ASPECT_RATIO);

}

#undef INTRO_ANIM_TIME
#undef INTRO_HOLD_TIME
#undef TITLE_ASPECT_RATIO

static void EnforceTitleAspectRatio(SDL_FRect *rect, const float wX, const float aspectRatio) 
{
	if ((rect->w / rect->h) < aspectRatio) {
		rect->w = rect->h * aspectRatio;
		if(rect->w > wX) {
			rect->w = wX;
			rect->h = rect->w / aspectRatio;
		}
	}
}

static void UpdateStepAfterAnim(IntroStep *introStep)
{
	if (*introStep == INTRO_STEP_ANIM) {
		*introStep = INTRO_STEP_TRANSITION;
	} else {
		*introStep = INTRO_STEP_HOLD;
	}
}



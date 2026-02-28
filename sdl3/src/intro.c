#include "intro.h"

#include <stdio.h>
#include <stdlib.h>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "colors.h"
#include "context.h"
#include "render.h"
#include "state_data.h"
#include "state_resources.h"
#include "types.h"

#define INTRO_ANIM_TIME 1000.00f
#define INTRO_HOLD_TIME  500.00f

static void NoneAnim(SDL_FRect destRect, const Vector2 windowSize, const u64 deltaTime, double angle);
static void ZoomAnim(SDL_FRect destRect, const Vector2 windowSize, const u64 deltaTime, double angle);

static const IntroAnimFunc IntroAnimTable[] = {
	[INTRO_ANIM_NONE]        = NoneAnim,
	[INTRO_ANIM_ZOOM]        = ZoomAnim,
//	[INTRO_ANIM_SLIDE_NORTH] = SlideNorthAnim,
//	[INTRO_ANIM_SLIDE_SOUTH] = SlideSouthAnim,
//	[INTRO_ANIM_SLIDE_EAST]  = SlideEastAnim,
//	[INTRO_ANIM_SLIDE_WEST]  = SlideWestAnim,
//	[INTRO_ANIM_SWIRL]       = SwirlAnim,
//	[INTRO_ANIM_LOOP]        = LoopAnim,
};

//CORE FOUR GLOBAL FUNCS

void Intro_Init(GameEngine *eng, GameData *data)
{

	//Create intro resources
	eng->stateResources = calloc(1, sizeof(IntroResources));
	if (eng->stateResources == NULL) {
		//TODO ERROR!
		return;
	}

	//Create intro state data
	data->stateData = calloc(1, sizeof(IntroData));
	if (data->stateData == NULL) {
		//TODO ERROR!
		return;
	}
	
	//local pointers
	IntroData *introData = data->stateData;
	IntroResources *introResources = eng->stateResources;

	//Load Resources
	introResources->title = TTF_CreateText(eng->textEngine, eng->font, "GRIDDY", 0);
	TTF_SetTextColor(introResources->title, 255, 0, 0, 255);
	introResources->titleTargetTexture = CreateTextureFromText(eng->renderer, introResources->title);

	//Load Data
	introData->startTime = SDL_GetTicks();
	//For testing purposes I will test them each individually THEN confirm the rand works
	introData->introAnim = INTRO_ANIM_ZOOM;
//	introData->introAnim = (rand() %  (INTRO_ANIM_COUNT - INTRO_ANIM_ZOOM)) + INTRO_ANIM_ZOOM;

}

//This should be a "mirror" of the init function - top bottom -> bottom top
void Intro_Cleanup(GameEngine *eng, GameData *data)
{
	//Make sure you clean up all your memory. Double check this stuff
	
	//Local pointers
//	IntroData *introData = data->stateData;
	IntroResources *introResources = eng->stateResources;
	
	//Cleanup Data - Nothing to do rn. 
	
	//Cleanup Resources
	TTF_DestroyText(introResources->title);
	SDL_DestroyTexture(introResources->titleTargetTexture);

	//Free allocd memory and NULL pointers

	//State data
	free(data->stateData);
	data->stateData = NULL;
	
	//State resources
	free(eng->stateResources);
	eng->stateResources = NULL;
}

void Intro_Update(const GameInput *input, GameData *data)
{
	(void)input;

	//local stateData pointer
	IntroData *introData = data->stateData;

	u64 currentTime = SDL_GetTicks();
	u64 deltaTime = currentTime - introData->startTime;
	__attribute__((unused))float holdTime = 500;

	//Handle specific Intro Anims - Index Table pls
	
	if (introData->introAnim == INTRO_ANIM_ZOOM) {
		IntroAnimFunc animFunc = IntroAnimTable[introData->introAnim];
		if (animFunc) {
			animFunc(introData->titleDestRect, data->windowSize, deltaTime, introData->textureRotation);
		}
	}
/* Move all this to ZoomAnim
	//Should we use deltaTime for scale or is it already 'sclaed' enough
	if (deltaTime < introTime) {
		scale = (float)deltaTime / introTime;
		introData->introStep = INTRO_STEP_INTRO;
	} else {
		scale = 1.0;
		if (introData->introStep == INTRO_STEP_INTRO) {
			introData->introStep = INTRO_STEP_TRANSITION;
		} else {
		introData->introStep = INTRO_STEP_HOLD;
		}
	}

	//Assign correct data to the target rec
	introData->titleDestRect.w = (float)wX * scale / 2.0f;
	introData->titleDestRect.h = (float)wY * scale / 2.0f;
	
	//Ensure rectangle is wider than it is tall to accomodate 6 letter string - 6:4 = 3:2 rect
	float aspectRatio = 6.0 / 4.0;
	if ((introData->titleDestRect.w / introData->titleDestRect.h) < aspectRatio) {
		
		//rectW minimum aspect ratio
		introData->titleDestRect.w = introData->titleDestRect.h * aspectRatio;

		//check rectW not too wide
		if (introData->titleDestRect.w > (wX - (wX) / 10.0f)) {
			introData->titleDestRect.w = wX - (wX / 10.0f);
			introData->titleDestRect.h = introData->titleDestRect.w / aspectRatio;
		}
	}

	//center rect
	introData->titleDestRect.x = (wX / 2.0f) - (introData->titleDestRect.w / 2.0f);
	introData->titleDestRect.y = (wY / 2.0f) - (introData->titleDestRect.h / 2.0f);
	*/
}

void Intro_Render(const GameEngine *eng, const GameData *data)
{
	IntroData *introData = data->stateData;
	IntroResources *introResources = eng->stateResources;
	SDL_Color bgColor = {0}, fgColor = {0};
	
	switch (introData->introStep) {
		case INTRO_STEP_INTRO:
			bgColor = COLOR_BLACK;
			fgColor = COLOR_WHITE;
			break;
		case INTRO_STEP_TRANSITION:
			//TODO update text string
			bgColor = COLOR_WHITE;
			fgColor = COLOR_BLACK;
			break;
		case INTRO_STEP_HOLD:
			bgColor = COLOR_WHITE;
			fgColor = COLOR_BLACK;
			break;
		default:
			//TODO error
			break;
	}

	//Background
	Render_SetDrawColor(eng->renderer, bgColor);
	SDL_RenderClear(eng->renderer);

	//(void)fgColor;
	//Foreground
	Render_SetDrawColor(eng->renderer, fgColor);
	SDL_RenderFillRect(eng->renderer, &introData->titleDestRect);

	//text
	SDL_RenderTexture (eng->renderer, introResources->titleTargetTexture, NULL, &introData->titleDestRect);
}

// STATIC FUNCS

static void NoneAnim(SDL_FRect destRect, const Vector2 windowSize, const u64 deltaTime, double angle) 
{
	(void)destRect;
	(void)windowSize;
	(void)deltaTime;
	(void)angle;
}

static void ZoomAnim(SDL_FRect destRect, const Vector2 windowSize, const u64 deltaTime, double angle) 
{
	(void)destRect;
	(void)windowSize;
	(void)deltaTime;
	(void)angle;
}

#undef INTRO_ANIM_TIME
#undef INTRO_HOLD_TIME

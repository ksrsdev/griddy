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



void IntroTick(const GameInput *input, GameData *data)
{
	(void)input;

	//local stateData pointer
	IntroData *introData = data->stateData;

	float scale = 0;
	uint64_t currentTime = SDL_GetTicks();
	uint64_t deltaTime = currentTime - introData->startTime;
	float introTime = 1000;
//	float holdTime = 500;

	//Should we use deltaTime for scale or it already 'sclaed' enough
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
	introData->titleDestRect.w = (float)data->windowSize.x * scale / 2.0f;
	introData->titleDestRect.h = (float)data->windowSize.y * scale / 2.0f;
	introData->titleDestRect.x = ((float)data->windowSize.x / 2.0f) - (introData->titleDestRect.w / 2.0f);
	introData->titleDestRect.y = ((float)data->windowSize.y / 2.0f) - (introData->titleDestRect.h / 2.0f);
}

void IntroRender(const GameEngine *eng, const GameData *data)
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

void IntroInit(GameEngine *eng, GameData *data)
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
	introResources->title = TTF_CreateText(eng->textEngine, eng->font, "TEST", 0);
	if (eng->font == NULL) {
		printf("FONT ERROR");
	}
	TTF_SetTextColor(introResources->title, 255, 0, 0, 255);
	introResources->titleTargetTexture = CreateTextureFromText(eng->renderer, introResources->title);

	//Load Data
	introData->startTime = SDL_GetTicks();

}

//This should be a "mirror" of the init function - top bottom -> bottom top
void IntroCleanup(GameEngine *eng, GameData *data)
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

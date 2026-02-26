#include "intro.h"

#include <stdio.h>
#include <stdlib.h>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "colors.h"
#include "context.h"
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
	} else if (deltaTime == introTime) {
		introData->introStep = INTRO_STEP_TRANSITION;
	} else {
		introData->introComplete = true;
		scale = 1.0;
		introData->introStep = INTRO_STEP_HOLD;
	}

	//Assign correct data to the target rec
	introData->titleDestRect.w = (float)data->windowSize.x * scale / 2.0f;
	introData->titleDestRect.h = (float)data->windowSize.y * scale / 2.0f;
	introData->titleDestRect.x = ((float)data->windowSize.x / 2.0f) - (introData->titleDestRect.w / 2.0f);
	introData->titleDestRect.y = ((float)data->windowSize.y / 2.0f) - (introData->titleDestRect.h / 2.0f);
}

void IntroRender(const GameEngine *eng, const GameData *data)
{
	//local stateData pointer
	IntroData *introData = data->stateData;
	
	SDL_SetRenderDrawColor(eng->renderer, 45, 45, 45, 255);
	SDL_RenderFillRect(eng->renderer, &introData->titleDestRect);

	//TODO text

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
	SDL_DestroyTexture(introResources->titleTargetTexture);
	TTF_DestroyText(introResources->title);

	//Free allocd memory and NULL pointers

	//State data
	free(data->stateData);
	data->stateData = NULL;
	
	//State resources
	free(eng->stateResources);
	eng->stateResources = NULL;
}

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
	IntroData *introData = (IntroData*)data->stateData;

	//TODO: Implement local pointer throughout

	float scale = 0;
	uint64_t currentTime = SDL_GetTicks();
	uint64_t deltaTime = currentTime - introData->startTime;
	float introTime = 1000;
//	float displayTime = 500;

	//Should we use deltaTime for scale or it already 'sclaed' enough
	if (deltaTime < introTime) {
		scale = (float)deltaTime / introTime;
		data->stateDate->introStep = INTRO_STEP_INTRO;
	} else if (deltaTime == introTime) {
		data->stateData->introStep = INTRO_STEP_TRANSITION;
	} else {
		data->stateData->introComplete = true;
		scale = 1.0;
		data->stateData->introStep = INTRO_STEP_HOLD;
	}

	//Assign correct data to the target rec
	data->stateDate->titleDestRect.w = (float)data->windowSize.x * scale / 2.0f;
	data->stateDate->titleDestRect.h = (float)data->windowSize.y * scale / 2.0f;
	data->stateDate->titleDestRect.x = ((float)data->windowSize.x / 2.0f) - (data->stateData->titleDestRect.w / 2.0f);
	data->stateDate->titleDestRect.y = ((float)data->windowSize.y / 2.0f) - (data->stateData->titleDestRect.h / 2.0f);
}

void IntroRender(const GameEngine *eng, const GameData *data)
{
	SDL_SetRenderDrawColor(eng->renderer, 45, 45, 45, 255);
	SDL_RenderFillRect(eng->renderer, &data->stateData->titleDestRect);

	//text

}

void IntroInit(GameEngine *eng, GameData *data)
{

	//Create intro resources
	eng->stateResources = calloc(1, sizeof(IntroResources));

	//Create intro state data
	data->stateData = calloc(1, sizeof(IntroData));

	//FIXME: I think data only has a destRec and eng holds the text now
	//data->stateData->title.text = TTF_CreateText(eng->textEngine, eng->font, "TEST", 0);
	data->stateData->startTime = SDL_GetTicks();
}

void IntroCleanup(GameEngine *eng, GameData *data)
{
	//TODO: Make sure you clean up all your memory. Double check this stuff
	(void)eng;

	//cleanup text object
	TTF_DestroyText(data->stateData->.title.text);

	//cleanup text target texture
	if (data->stateData->.titleTargetTexture != NULL) {
		SDL_DestroyTexture(data->stateData->.titleTargetTexture);
		data->stateData->.titleTargetTexture = NULL;
	}

	//cleanup state data
	free(data->stateData);
	data->stateData = NULL;
	
	//cleanup state resources
	free(eng->stateResources);
	data->stateResources = NULL;
}

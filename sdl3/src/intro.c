#include <stdio.h>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "context.h"
#include "intro.h"



void IntroTick(const GameInput *input, GameData *data)
{
	(void)input;

	float scale = 0;
	uint64_t currentTime = SDL_GetTicks();
	uint64_t deltaTime = currentTime - data->layout.intro.startTime;
	float introTime = 1000;
//	float displayTime = 500;

	//Should we use deltaTime for scale or it already 'sclaed' enough
	if (deltaTime < introTime) {
		scale = (float)deltaTime / introTime;
	} else {
		data->layout.intro.introComplete = true;
		scale = 1.0;
	}

	//Assign correct data to the target rec
	data->layout.intro.rect.w = (float)data->windowSize.x * scale / 2.0f;
	data->layout.intro.rect.h = (float)data->windowSize.y * scale / 2.0f;
	data->layout.intro.rect.x = ((float)data->windowSize.x / 2.0f) - (data->layout.intro.rect.w / 2.0f);
	data->layout.intro.rect.y = ((float)data->windowSize.y / 2.0f) - (data->layout.intro.rect.h / 2.0f);
}

void IntroRender(const GameEngine *eng, const GameData *data)
{
	SDL_SetRenderDrawColor(eng->renderer, 45, 45, 45, 255);
	SDL_RenderFillRect(eng->renderer, &data->layout.intro.rect);

	//text

}

//TODO
void IntroInit(GameEngine *eng, GameData *data)
{
	(void)eng;
	data->layout.intro.title.text = TTF_CreateText(eng->textEngine, eng->font, "TEST", 0);
	data->layout.intro.startTime = SDL_GetTicks();
}

void IntroCleanup(GameEngine *eng, GameData *data)
{
	(void)eng;
	//cleanup text object
	TTF_DestroyText(data->layout.intro.title.text);
	//cleanup text target texture
	if (data->layout.intro.titleTargetTexture != NULL) {
		SDL_DestroyTexture(data->layout.intro.titleTargetTexture);
		data->layout.intro.titleTargetTexture = NULL;
	}
}

#include <stdio.h>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "context.h"
#include "intro.h"

//TODO
void Intro_Init(GameData *data)
{
	data->layout.intro.title.text = TTF_CreateText(data->textEngine, data->font, "TEST", 0);
}

void Intro_Tick(const GameInput *input, GameData *data)
{
	(void)input;

	//Variable Declaration
	float scale = 0;
	uint64_t currentTime = SDL_GetTicks();
	uint64_t deltaTime = currentTime - data->layout.intro.startTime;

	//Should we use deltaTime for scale or it already 'sclaed' enough
	if (deltaTime < 2000) {
		scale = (float)deltaTime / 2000.0f;
	} else {
		printf("YUP\n");
		scale = 1.0;
	}

	//Assign correct data to the target rec
	data->layout.intro.rect.w = (float)data->windowSize.x * scale / 2.0f;
	data->layout.intro.rect.h = (float)data->windowSize.y * scale / 2.0f;
	data->layout.intro.rect.x = ((float)data->windowSize.x / 2.0f) - (data->layout.intro.rect.w / 2.0f);
	data->layout.intro.rect.y = ((float)data->windowSize.y / 2.0f) - (data->layout.intro.rect.h / 2.0f);
}

void Intro_Render(const GameEngine *eng, const GameData *data)
{
	SDL_SetRenderDrawColor(eng->renderer, 45, 45, 45, 255);
	SDL_RenderFillRect(eng->renderer, &data->layout.intro.rect);
}

void Intro_Cleanup(GameData *data)
{
	TTF_DestroyText(data->layout.intro.title.text);
}

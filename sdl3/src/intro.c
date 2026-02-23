#include <stdio.h>

#include <SDL3/SDL.h>

#include "context.h"
#include "intro.h"

void Tick_Intro(const GameInput *input, GameData *data)
{
//	printf("tickCounter: %ld\n", data->tickCounter);
	(void)input;
	uint64_t currentTime = SDL_GetTicks();
	uint64_t deltaTime = currentTime - data->tickCounter;
	printf("deltaTime: %ld\n", deltaTime);
	float scale = 0;
	if (deltaTime < 2000) {
		scale = (float)deltaTime / 2000.0f;
	} else {
		printf("YUP\n");
		scale = 1.0;
	}
	data->layout.intro.rect.w = (float)data->windowSize.x * scale / 2.0f;
	data->layout.intro.rect.h = (float)data->windowSize.y * scale / 2.0f;
	data->layout.intro.rect.x = ((float)data->windowSize.x / 2.0f) - (data->layout.intro.rect.w / 2.0f);
	data->layout.intro.rect.y = ((float)data->windowSize.y / 2.0f) - (data->layout.intro.rect.h / 2.0f);
}

void Render_Intro(const GameEngine *eng, const GameData *data)
{
	printf("Render_Intro()\n");
	SDL_SetRenderDrawColor(eng->renderer, 45, 45, 45, 255);
	SDL_RenderFillRect(eng->renderer, &data->layout.intro.rect);
}

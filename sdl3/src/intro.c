#include <stdio.h>

#include <SDL3/SDL.h>

#include "context.h"
#include "intro.h"


void Tick_Intro(const GameInput *input, GameData *data)
{
	printf("tickCounter: %d\n", data->tickCounter);
	(void)input;
	int maxRecSize = data->windowSize.x / 2;
//	printf("maxRecSize: %d\n", maxRecSize);
	int counter = data->tickCounter;
	if (counter < maxRecSize) {
		data->tickCounter++;
	} else {
		printf("tickCounter MAX\n");
	}
	data->layout.intro.rect.w = (float)counter;
	data->layout.intro.rect.h = (float)counter;
	data->layout.intro.rect.x = ((float)data->windowSize.x / 2.0f) - ((float)counter / 2.0f);
	data->layout.intro.rect.y = ((float)data->windowSize.y / 2.0f) - ((float)counter / 2.0f);
}

void Render_Intro(const GameEngine *eng, const GameData *data)
{
	printf("Render_Intro()\n");
	SDL_SetRenderDrawColor(eng->renderer, 45, 45, 45, 255);
	SDL_RenderFillRect(eng->renderer, &data->layout.intro.rect);
}

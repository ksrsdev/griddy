#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "context.h"
#include "render.h"

static void ClearScreen(SDL_Renderer *renderer)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
}

void Render_Core(GameEngine *eng, GameData *data)
{
	//NOTE: This function should be DUMB - NO THINKING!

	//Placeholder!
	(void)data;
	//Clear screen
	ClearScreen(eng->renderer);
	//Draw correct screen

	//Final before return
	SDL_RenderPresent(eng->renderer);
}

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "context.h"
#include "render.h"

static void ClearScreen(Context *ctx)
{
	SDL_SetRenderDrawColor(ctx->renderer, 0, 0, 0, 255);
	SDL_RenderClear(ctx->renderer);
}

void Render_Core(Context *ctx)
{
	ClearScreen(ctx);

	//Final before return
	SDL_RenderPresent(ctx->renderer);
}

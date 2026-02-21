#include <stdio.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "context.h"
#include "core.h"
#include "input.h"
#include "render.h"

static int InitSDLSubsystems(void)
{
	//Init SDL Video
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("SDL_Init Error: %s", SDL_GetError());
		return 1;
	}
	//Init SDL TTF
	if (!TTF_Init()) {
		SDL_Log("SDL_Init Error: %s", SDL_GetError());
		return 1;
	}
	return 0;
}

static void QuitSDLSubsystems(void)
{
	TTF_Quit();
	SDL_Quit();
}

static void CleanupContextStruct(Context *ctx)
{
	if (ctx->renderer != NULL) {
		SDL_DestroyRenderer(ctx->renderer);
	}
	if (ctx->window != NULL) {
		SDL_DestroyWindow(ctx->window);
	}
	if (ctx->textEngine != NULL) {
		TTF_DestroyRendererTextEngine(ctx->textEngine);
	}
}

static int InitCtxSDLObjs(Context *ctx)
{
	SDL_SetHint(SDL_HINT_VIDEO_WAYLAND_ALLOW_LIBDECOR, "0"); 
	ctx->window = SDL_CreateWindow("Hello SDL", 960, 540, SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);
	ctx->renderer = SDL_CreateRenderer(ctx->window, NULL);
	ctx->textEngine = TTF_CreateRendererTextEngine(ctx->renderer);
	return 0;
}

static Context InitContext(void)
{
	Context ctx = {
		.window = NULL,
		.renderer = NULL,
		.textEngine = NULL,
		.isRunning = true,
		.state = MAIN_GAME_STATE_NONE,
		.prevState = MAIN_GAME_STATE_NONE,
	};
	return ctx;
}

int main(void)
{
	//Init ctx
	Context ctx = InitContext();
	if (InitSDLSubsystems() == 1) {
		SDL_Quit();
		return 1;
	}
	//Create SDL Vars (window, renderer, textEngine)
	InitCtxSDLObjs(&ctx);
	//throttle cpu
	SDL_SetRenderVSync(ctx.renderer, 1);
	//Loop
	while (ctx.isRunning) {
		//Input
		Input_PollEvents(&ctx);
		//Logic
		Core_Tick(&ctx);
		//Draw
		Render_Core(&ctx);
	}
	CleanupContextStruct(&ctx);
	QuitSDLSubsystems();
	return 0;
}

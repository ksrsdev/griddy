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

static void CleanupContextStruct(GameEngine *eng)
{
	if (eng->renderer != NULL) {
		SDL_DestroyRenderer(eng->renderer);
	}
	if (eng->window != NULL) {
		SDL_DestroyWindow(eng->window);
	}
	if (eng->textEngine != NULL) {
		TTF_DestroyRendererTextEngine(eng->textEngine);
	}
}

static int InitCtxSDLObjs(GameEngine *eng)
{
	SDL_SetHint(SDL_HINT_VIDEO_WAYLAND_ALLOW_LIBDECOR, "0"); 
	eng->window = SDL_CreateWindow("Hello SDL", 960, 540, SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);
	eng->renderer = SDL_CreateRenderer(eng->window, NULL);
	eng->textEngine = TTF_CreateRendererTextEngine(eng->renderer);
	return 0;
}

static Context InitContext(void)
{
	GameEngine eng = {
		.window = NULL,
		.renderer = NULL,
		.textEngine = NULL,
	};

	GameData data = {
		.isRunning = true,
		.state = MAIN_GAME_STATE_NONE,
		.prevState = MAIN_GAME_STATE_NONE,
		.errorMsg = {0},
		.isErrorFatal = false
	};

	GameInput input = {
		.mouseX = 0,
		.mouseY = 0,
		.mouseButtonDown = false,
		.mouseButtonPressed = false,
		.windowResized = false,
		.newWindowWidth = 0,
		.newWindowHeight = 0,
		.keys = {0},
	};

	Context ctx = {
		.eng = eng,
		.input = input,
		.data = data,
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
	InitCtxSDLObjs(&ctx.eng);
	//throttle cpu
	SDL_SetRenderVSync(ctx.eng.renderer, 1);
	//Loop
	while (ctx.data.isRunning) {
		//Input
		Input_PollEvents(&ctx.eng, &ctx.input);
		//Logic
		Core_Tick(&ctx.input, &ctx.data);
		//Draw
		Render_Core(&ctx);
	}
	CleanupContextStruct(&ctx.eng);
	QuitSDLSubsystems();
	return 0;
}

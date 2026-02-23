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
	GameInput input = {
		.mouseButtonDown = false,
		.mouseButtonPressed = false,
		.windowResized = false,
		.quitRequested = false,
		.newWindowSize = {0},
		.mousePos = {0},
		.keys = {0},
	};
	GameData data = {
		.isRunning = true,
		.state = GAME_STATE_NONE,
		.prevState = GAME_STATE_NONE,
		.errorMsg = {0},
		.windowSize = {0},
		.isErrorFatal = false,
		.tickCounter = 0,
	};
	Context ctx = {
		.eng = eng,
		.input = input,
		.data = data,
	};
	return ctx;
}

static void InitGameData(GameEngine *eng, GameData *data)
{
	//Set GameData resolution
	int winW, winH;
	SDL_GetWindowSize(eng->window, &winW, &winH);
	data->windowSize.x = winW;
	data->windowSize.y = winH;
}

static void  WaitForFirstFrame(SDL_Renderer *renderer)
{
	// Force the renderer to actually "talk" to the GPU once
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
	// Small delay to let the OS catch its breath
	SDL_Delay(100); 
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
	//Wait for renderer and window to be actually ready
	WaitForFirstFrame(ctx.eng.renderer);
	//Init Global vars 
	InitGameData(&ctx.eng, &ctx.data);
	//Main Loop
	while (ctx.data.isRunning) {
		//Input
		Input_PollEvents(&ctx.eng, &ctx.input);
		//Logic
		Core_Tick(&ctx.input, &ctx.data);
		//Draw
		Render_Core(&ctx.eng, &ctx.data);
	}
	CleanupContextStruct(&ctx.eng);
	QuitSDLSubsystems();
	return 0;
}

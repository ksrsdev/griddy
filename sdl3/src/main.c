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

	//TODO: Move to GameData
	if (eng->textEngine != NULL) {
		TTF_DestroyRendererTextEngine(eng->textEngine);
	}

	if (data->font != NULL) {
		TTF_CloseFont(data->font);
	}
}

static int InitEng(GameEngine *eng)
{
	SDL_SetHint(SDL_HINT_VIDEO_WAYLAND_ALLOW_LIBDECOR, "0"); 
	eng->window = SDL_CreateWindow("Hello SDL", 960, 540, SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);
	eng->renderer = SDL_CreateRenderer(eng->window, NULL);
	//TODO: Move to GameData
	eng->textEngine = TTF_CreateRendererTextEngine(eng->renderer);
	return 0;
}

static Context InitContext(void)
{
	GameEngine eng = {0};
	GameInput input = {0};
	GameData data = {0};

	//set variables that need non-junk data
	data.isRunning = true;
	data.state = GAME_STATE_NONE;
	data.prevState = GAME_STATE_NONE;

	//Create global struct
	Context ctx = {
		.eng = eng,
		.input = input,
		.data = data,
	};

	return ctx;
}

//Set GameData resolution
static void InitGameData(GameEngine *eng, GameData *data)
{
	//Window Size
	int winW, winH;
	SDL_GetWindowSize(eng->window, &winW, &winH);
	data->windowSize.x = winW;
	data->windowSize.y = winH;

	//Font
	data->font = TTF_OpenFont("fonts/Press_Start_2P/PressStart2P-Regular.ttf", 64);
	TTF_SetFontSDF(data->font, true);
}

static void  WaitForFirstFrame(SDL_Renderer *renderer)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
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
	InitEng(&ctx.eng);

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
	
	//Exit stuff
	CleanupContextStruct(&ctx);
	QuitSDLSubsystems();

	return 0;
}

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "context.h"
#include "core.h"
#include "input.h"
#include "render.h"
#include "state_manager.h"

//   ***   STATIC FUNCTION DECLARATIONS   ***  

static Context InitContext(void);
static int InitSDLSubsystems(void);
static int InitEng(GameEngine *eng);
static void WaitForFirstFrame(SDL_Renderer *renderer);
static void InitGameData(GameEngine *eng, GameData *data);
static void CleanupContextStruct(Context *ctx);
static void QuitSDLSubsystems(void);

//   ***   FUNCTION DEFINITIONS   ***  

int main(void)
{
	//Init ctx
	Context ctx = InitContext();

	//Init SDL
	if (InitSDLSubsystems() == 1) {
		SDL_Quit();
		return 1;
	}

	//Create SDL Objects (window, renderer, textEngine)
	InitEng(&ctx.eng);

	//Wait for renderer and window to be actually ready
	WaitForFirstFrame(ctx.eng.renderer);

	//Init Global vars 
	InitGameData(&ctx.eng, &ctx.data);

	//Main Loop
	while (ctx.data.isRunning) {
		//Input
		InputPollEvents(&ctx.eng, &ctx.input);
	
		//Logic
		CoreTick(&ctx.input, &ctx.data);
	
		//Draw
		RenderCore(&ctx.eng, &ctx.data);
		
		//StateManager - cleanup, confirm valid, assign, init new state
		if (ctx.data.newState != ctx.data.currState) {
			StateManager(&ctx.eng, &ctx.data);
		}
	}
	
	//Exit stuff
	CleanupCurrentState(&ctx.eng, &ctx.data);
	CleanupContextStruct(&ctx);
	QuitSDLSubsystems();

	return 0;
}

static Context InitContext(void)
{
	GameEngine eng = {0};
	GameInput input = {0};
	GameData data = {0};

	//set variables that need non-junk data
	data.isRunning = true;
	data.newState = GAME_STATE_NONE;
	data.currState = GAME_STATE_NONE;
	data.prevState = GAME_STATE_NONE;

	//Create global struct
	Context ctx = {
		.eng = eng,
		.input = input,
		.data = data,
	};

	return ctx;
}

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

static int InitEng(GameEngine *eng)
{
	SDL_SetHint(SDL_HINT_VIDEO_WAYLAND_ALLOW_LIBDECOR, "0"); 
	eng->window = SDL_CreateWindow("Hello SDL", 960, 540, SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);
	eng->renderer = SDL_CreateRenderer(eng->window, NULL);

	//throttle cpu
	SDL_SetRenderVSync(eng->renderer, 1);
	return 0;
}

static void WaitForFirstFrame(SDL_Renderer *renderer)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
	SDL_Delay(100); 
}

//Set GameData resolution
static void InitGameData(GameEngine *eng, GameData *data)
{
	//Window Size
	int winW, winH;
	SDL_GetWindowSize(eng->window, &winW, &winH);
	data->windowSize.x = winW;
	data->windowSize.y = winH;

	//TextEngine
	eng->textEngine = TTF_CreateRendererTextEngine(eng->renderer);
	eng->font = TTF_OpenFont("fonts/Press_Start_2P/PressStart2P-Regular.ttf", 64);
	TTF_SetFontSDF(eng->font, true);
}

static void CleanupContextStruct(Context *ctx)
{
	//Engine Cleanup
	if (ctx->eng.renderer != NULL) {
		SDL_DestroyRenderer(ctx->eng.renderer);
	}
	if (ctx->eng.window != NULL) {
		SDL_DestroyWindow(ctx->eng.window);
	}

	//Text Engine Cleanup
	if (ctx->eng.textEngine != NULL) {
		TTF_DestroyRendererTextEngine(ctx->eng.textEngine);
	}
	if (ctx->eng.font != NULL) {
		TTF_CloseFont(ctx->eng.font);
	}
}

static void QuitSDLSubsystems(void)
{
	TTF_Quit();
	SDL_Quit();
}

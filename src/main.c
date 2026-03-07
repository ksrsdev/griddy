#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "context.h"
#include "input.h"
#include "render.h"
#include "state_manager.h"
#include "update.h"

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
	//Set random seed - only once per program execution
	srand((unsigned int)time(NULL));

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
		Main_Update(&ctx.input, &ctx.data);
	
		//Draw
		Main_Render(&ctx.eng, &ctx.data);
		
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
	eng->window = SDL_CreateWindow("Griddy", 960, 540, SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);
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
	data->window.size.x = winW;
	data->window.size.y = winH;

	//Mouse
	float mX, mY;
	SDL_GetMouseState(&mX, &mY);
	data->mouse.pos.x = mX;
	data->mouse.pos.y = mY;

	//TextEngine
	eng->textEngine = TTF_CreateRendererTextEngine(eng->renderer);
	if (!eng->textEngine) {
		printf("ERROR: text Engine not created!\n");
	}

	eng->font = TTF_OpenFont("assets/fonts/Press_Start_2P/PressStart2P-Regular.ttf", 128);
	if (!eng->font) {
		printf("ERROR: font not loaded\n");
	}
}

static void CleanupContextStruct(Context *ctx)
{
	//Text Engine Cleanup
	if (ctx->eng.textEngine != NULL) {
		TTF_DestroyRendererTextEngine(ctx->eng.textEngine);
	}
	if (ctx->eng.font != NULL) {
		TTF_CloseFont(ctx->eng.font);
	}
	
	//Engine Cleanup
	if (ctx->eng.renderer != NULL) {
		SDL_DestroyRenderer(ctx->eng.renderer);
	}
	if (ctx->eng.window != NULL) {
		SDL_DestroyWindow(ctx->eng.window);
	}
}

static void QuitSDLSubsystems(void)
{
	TTF_Quit();
	SDL_Quit();
}

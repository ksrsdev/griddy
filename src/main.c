#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <SDL3_shadercross/SDL_shadercross.h>

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "context.h"
#include "init.h"
#include "input.h"
#include "post_update.h"
#include "render.h"
#include "state_manager.h"
#include "text.h"
#include "sync_input.h"
#include "update.h"

//   ***   STATIC FUNCTION DECLARATIONS   ***  

static void WaitForFirstFrame(SDL_Renderer *renderer);

//   ***   FUNCTION DEFINITIONS   ***  

int main(void)
{
	//Set random seed - only once per program execution
	srand((unsigned int)time(NULL));

	//Init ctx
	Context ctx = InitContext();

	//Init SDL
	if (!Init_SDLSubsystems()) {
		SDL_Quit();
		return 1;
	}

	//Create SDL Objects (window, renderer, textEngine)
	if (!Init_GameEngine(&ctx.eng)) {
		Deinit_GameEngine(&ctx.eng);
		Deinit_SDLSubsystems();
		SDL_Quit();
		return 1;
	}

	//Wait for renderer and window to be actually ready
	WaitForFirstFrame(ctx.eng.renderer);

	//Init Global vars 
	Init_GameData(&ctx.eng, &ctx.data);

	//Main Loop
	while (ctx.data.isRunning) {
		//Input
		Input_PollEvents(&ctx.eng, &ctx.input);
		if (ctx.input.quitRequested) {
			break;
		}

		//Sync Input
		Main_SyncInput(&ctx.input, &ctx.data);
		if (!ctx.data.isRunning) {
			break;
		}
	
		//Logic
		Main_Update(&ctx.data);
		if (!ctx.data.isRunning) {
			break;
		}

		//new textures, sub state init / cleanup, etc
		Main_PostUpdate(&ctx.eng, &ctx.data);
	
		//Draw
		Main_Render(&ctx.eng, &ctx.data);
		
		//StateManager - cleanup, confirm valid, assign, init new state
		if (ctx.data.state.next != ctx.data.state.curr) {
			StateManager(&ctx.eng, &ctx.data);
		}
	}
	
	//Exit stuff
	CleanupCurrentState(&ctx.eng, &ctx.data);
	Deinit_GameEngine(&ctx.eng);
	Deinit_SDLSubsystems();

	return 0;
}

static void WaitForFirstFrame(SDL_Renderer *renderer)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
	SDL_FlushRenderer(renderer);
	SDL_Delay(100); 
}


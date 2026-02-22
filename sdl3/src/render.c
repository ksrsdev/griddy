#include <stdio.h>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "context.h"
#include "intro.h"
#include "render.h"

//Should be in respective .c file methinks
void Render_None(const GameEngine *eng, const GameData *data);
void Render_Error(const GameEngine *eng, const GameData *data);
void Render_MainMenu(const GameEngine *eng, const GameData *data);
//Should be in respective .c file methinks
void Render_None(const GameEngine *eng, const GameData *data)
{
	(void)eng;
	(void)data;
}
void Render_Error(const GameEngine *eng, const GameData *data)
{
	(void)eng;
	(void)data;
}
void Render_MainMenu(const GameEngine *eng, const GameData *data)
{
	(void)eng;
	(void)data;
}

static const RenderFunc RenderTable[] = {
	[GAME_STATE_NONE]      = Render_None,
	[GAME_STATE_ERROR]     = Render_Error,
	[GAME_STATE_INTRO]     = Render_Intro,
	[GAME_STATE_MAIN_MENU] = Render_MainMenu,
};

static void ClearScreen(SDL_Renderer *renderer)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
}

//NOTE: This function should be DUMB - NO THINKING!
void Render_Core(const GameEngine *eng, const GameData *data)
{
	//Clear screen
	ClearScreen(eng->renderer);
	//Draw correct screen
	if (data->state >= GAME_STATE_NONE && data->state < GAME_STATE_COUNT) {
		RenderFunc renderFunc = RenderTable[data->state];
		if (renderFunc) {
			renderFunc(eng, data);
		}
	} else {
		printf("ERROR: GameState: %d OOB in Render_Core()\n", data->state);
	}
	//Final before return
	SDL_RenderPresent(eng->renderer);
}

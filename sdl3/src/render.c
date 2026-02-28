#include "render.h"

#include <stdio.h>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "context.h"
#include "error.h"
#include "intro.h"
#include "main_menu.h"

//   ***   STATIC FUNCTION DECLARATIONS   ***  

static void ClearScreen(SDL_Renderer *renderer);
static void NoneRender(const GameEngine *eng, const GameData *data);

//   ***   LOOKUP TABLES   ***  

static const RenderFunc RenderTable[] = {
	[GAME_STATE_NONE]      = NoneRender,
	[GAME_STATE_ERROR]     = ErrorRender,
	[GAME_STATE_INTRO]     = Intro_Render,
	[GAME_STATE_MAIN_MENU] = MainMenuRender,
};

//   ***   FUNCTION DEFINITIONS   ***  

void Main_Render(const GameEngine *eng, const GameData *data)
{
	//Clear screen
	ClearScreen(eng->renderer);

	//Draw correct screen
	if (data->currState >= GAME_STATE_NONE && data->currState < GAME_STATE_COUNT) {
		RenderFunc renderFunc = RenderTable[data->currState];
		if (renderFunc) {
			renderFunc(eng, data);
		}
	} else {
		printf("ERROR: GameState: %d OOB in Render_Core()\n", data->currState);
	}

	//Final before return
	SDL_RenderPresent(eng->renderer);
}

//use colors.h values to change renderer color
void Render_SetDrawColor(SDL_Renderer *renderer, const SDL_Color color)
{
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

static void ClearScreen(SDL_Renderer *renderer)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
}

//TODO: This should return an error
void NoneRender(const GameEngine *eng, const GameData *data)
{
	(void)eng;
	(void)data;
}



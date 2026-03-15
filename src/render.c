#include "render.h"

#include <stdio.h>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "context.h"
#include "error.h"
#include "intro.h"
#include "main_menu.h"
#include "state_data.h"

//   ***   STATIC FUNCTION DECLARATIONS   ***  

static void ClearScreen(SDL_Renderer *renderer);
static void Render_UIElementOutline(SDL_Renderer *renderer, const UIData *data);
static void Render_RectRotated(SDL_Renderer *renderer, const SDL_FRect *destRect, const f64 rotation, const SDL_Color color);
static void None_Render(const GameEngine *eng, const GameData *data);

//   ***   LOOKUP TABLES   ***  

static const RenderFunc RenderTable[] = {
	[GAME_STATE_NONE]      = None_Render,
	[GAME_STATE_ERROR]     = Error_Render,
	[GAME_STATE_INTRO]     = Intro_Render,
	[GAME_STATE_MAIN_MENU] = MainMenu_Render,
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

void Render_SetupSDFRenderState(const GameEngine *eng, const SDL_Color color, SDL_Texture *targetTexture)
{
	SDL_SetGPURenderState(eng->renderer, eng->sdfRenderState);

	SDL_SetTextureColorMod(targetTexture, color.r, color.g, color.b);
	SDL_SetTextureAlphaMod(targetTexture, color.a);
}

//TODO make static
void Render_ResetRenderState(SDL_Renderer *renderer)
{
	SDL_SetGPURenderState(renderer, NULL);
}

void Render_UIElement(const GameEngine *eng, const UIData *data, SDL_Texture *texture)
{
	//ouline
	if (data->outlined && data->outlineColor.a != 0) {
		Render_UIElementOutline(eng->renderer, data);
	}
	
	//bg
	if (data->hasBackground && data->bg.a != 0) {
		if (data->rotation == 0) {
			Render_SetDrawColor(eng->renderer, data->bg);
			SDL_RenderFillRect(eng->renderer, &data->destRect);
		} else {
			Render_RectRotated(eng->renderer, &data->destRect, data->rotation, data->bg);
		}
	}

	//fg = text
	Render_SetupSDFRenderState(eng, data->fg, texture);
	if (data->rotation == 0) {
		SDL_RenderTexture(eng->renderer, texture, NULL,  &data->destRect);
	} else {
		SDL_RenderTextureRotated(eng->renderer, texture, NULL, &data->destRect, data->rotation, NULL, SDL_FLIP_NONE);
	}
	Render_ResetRenderState(eng->renderer);

}

#define OUTLINE_PIXEL_WIDTH 2.0f

static void Render_UIElementOutline(SDL_Renderer *renderer, const UIData *data)
{
	SDL_FRect outlineRect;
	outlineRect.w = data->destRect.w + (2.0f * OUTLINE_PIXEL_WIDTH);
	outlineRect.h = data->destRect.h + (2.0f * OUTLINE_PIXEL_WIDTH);
	outlineRect.x = data->destRect.x - OUTLINE_PIXEL_WIDTH;
	outlineRect.y = data->destRect.y - OUTLINE_PIXEL_WIDTH;
	Render_SetDrawColor(renderer, data->outlineColor);
	SDL_RenderFillRect(renderer, &outlineRect);
}

#undef OUTLINE_PIXEL_WIDTH

static void Render_RectRotated(SDL_Renderer *renderer, const SDL_FRect *destRect, const f64 rotation, const SDL_Color color)
{
	(void)renderer;
	(void)destRect;
	(void)rotation;
	(void)color;

}

void None_Render(const GameEngine *eng, const GameData *data)
{
	(void)eng;
	(void)data;
	//ERROR - this function should never be called so set error
}



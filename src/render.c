#include "render.h"

#include <math.h>
#include <stdio.h>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "context.h"
#include "error.h"
#include "intro.h"
#include "main_menu.h"
#include "state_data.h"
#include "types.h"
#include "util.h"

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

void Render_UIElement(const GameEngine *eng, const UIData *data)
{
	SDL_Texture *texture = data->texture;

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
	
	f32 padding = 4.0f;

	SDL_FRect paddedRect;
	paddedRect.x = data->destRect.x + padding;
	paddedRect.y = data->destRect.y + padding;
	// Ensure we don't collapse the box into a black hole
	paddedRect.w = MAX(0.0f, data->destRect.w - (padding * 2.0f));
	paddedRect.h = MAX(0.0f, data->destRect.h - (padding * 2.0f));

	f32 scaleX = paddedRect.w / (f32)texture->w;
	f32 scaleY = paddedRect.h / (f32)texture->h;

	f32 finalScale = MIN(scaleX, scaleY);

	SDL_FRect targetRect;
	targetRect.w = (f32)texture->w * finalScale;
	targetRect.h = (f32)texture->h * finalScale;
	targetRect.x = paddedRect.x + ((paddedRect.w - targetRect.w) * 0.5f);
	targetRect.y = paddedRect.y + ((paddedRect.h - targetRect.h) * 0.5f);
	Render_SetupSDFRenderState(eng, data->fg, texture);
	if (data->rotation == 0) {
		SDL_RenderTexture(eng->renderer, texture, NULL,  &targetRect);
	} else {
		SDL_RenderTextureRotated(eng->renderer, texture, NULL, &targetRect, data->rotation, NULL, SDL_FLIP_NONE);
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
	f32 radians = (float)rotation * DEG_TO_RAD;
	f32 cosA = cosf(radians);
	f32 sinA = sinf(radians);

	f32 halfW = destRect->w * 0.5f;
	f32 halfH = destRect->h * 0.5f;

	//clockwise from top right
	FVector2 corners[4] = {
		{halfW, halfH},
		{halfW, -halfH},
		{-halfW, -halfH},
		{-halfW, halfH}
	};

	SDL_Vertex vertices[4];
	for (u8 i = 0; i < 4; i++) {
		vertices[i].position.x = destRect->x + (corners[i].x * cosA - corners[i].y * sinA);
		vertices[i].position.y = destRect->y + (corners[i].x * sinA + corners[i].y * cosA);
		vertices[i].color.r = color.r / 255.0f;
		vertices[i].color.g = color.g / 255.0f;
		vertices[i].color.b = color.b / 255.0f;
		vertices[i].color.a = color.a / 255.0f;
	};

	s32 indices[] = {0, 1, 3, 1, 2, 3};

	SDL_RenderGeometry(renderer, NULL, vertices, 4, indices, 6);

}

void None_Render(const GameEngine *eng, const GameData *data)
{
	(void)eng;
	(void)data;
	//ERROR - this function should never be called so set error
}



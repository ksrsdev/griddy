#ifndef GUARD_TEXT_H
#define GUARD_TEXT_H

#include <stdbool.h>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "context.h"

typedef struct {
	SDL_FRect rect;
	TTF_Text *text;
} TextBox;

SDL_Texture* CreateTextureFromText(SDL_Renderer *renderer, TTF_Text *text);
void Text_SetColor(TTF_Text *text, SDL_Color color);
void Text_DrawCentered(TTF_Text *text, SDL_FRect *destRect);
SDL_Texture* CreateTextureFromText(SDL_Renderer *renderer, TTF_Text *text);
SDL_Texture* CreateTextureViaSurfaceFromText(SDL_Renderer *renderer, TTF_Font *font, const char* string);
bool Text_LoadFonts(GameFonts *fonts, const TextureScale textureScale);

#endif

#ifndef GUARD_TEXT_H
#define GUARD_TEXT_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

typedef struct {
	SDL_FRect rect;
	TTF_Text *text;
} TextBox;

SDL_Texture* CreateTextureFromText(SDL_Renderer *renderer, TTF_Text *text);

void Text_SetColor(TTF_Text *text, SDL_Color color);

#endif

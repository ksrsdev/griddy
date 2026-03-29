#ifndef GUARD_TEXT_H
#define GUARD_TEXT_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "context.h"
#include "ui.h"

#define TEXT_NO_WRAP 0.0f

typedef struct {
	SDL_FRect rect;
	TTF_Text *text;
} TextBox;

SDL_Texture* Text_CreateTextureFromText(SDL_Renderer *renderer, TTF_Text *text, const s32 textW, const s32 textH);
void Text_SetColor(TTF_Text *text, SDL_Color color);
SDL_Texture * Text_CreateUITexture(const GameEngine *eng, const char *string, UIData *uiData);

#endif

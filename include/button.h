#ifndef GUARD_BUTTON_H
#define GUARD_BUTTON_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "context.h"

typedef void (*ButtonPress)(GameData* data);

typedef struct {
	SDL_FRect rect;
	TTF_Text text;
	bool isHovered;
	ButtonPress onClick;
} Button;

typedef struct {
	SDL_FRect rect;
	bool isHovered;
	SDL_Color bg;
	SDL_Color textColor;
	ButtonPress onClick;
} ButtonData;

#endif

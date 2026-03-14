#ifndef GUARD_BUTTON_H
#define GUARD_BUTTON_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "context.h"

typedef void (*ButtonPress)(GameData* data);

typedef struct {
	bool isHovered;
	SDL_Color bg;
	ButtonPress onClick;
} ButtonData;

#endif

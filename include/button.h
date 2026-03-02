#ifndef GUARD_BUTTON_H
#define GUARD_BUTTON_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

typedef void (*ButtonPress)(void* data);

typedef struct {
	SDL_FRect rect;
	TTF_Text text;
	bool isHovered;
	ButtonPress onClick;
} Button;

#endif

#ifndef GUARD_BUTTON_H
#define GUARD_BUTTON_H

#include <SDL3/SDL.h>

typedef void (*ButtonPress)(void* data);

typedef struct {
	SDL_FRect rect;
	const char *text;
	bool isHovered;
	ButtonPress onClick;
} Button;

#endif

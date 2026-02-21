#ifndef GUARD_TEXT_H
#define GUARD_TEXT_H

#include <SDL3/SDL.h>

typedef struct {
	SDL_FRect rect;
	const char *text;
} TextBox;

#endif

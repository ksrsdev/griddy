#ifndef GUARD_CONTEXT_H
#define GUARD_CONTEXT_H

#include <stdbool.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>

typedef struct {
	bool shouldQuit;
	SDL_Window *window;
	SDL_Renderer *renderer;
	TTF_TextEngine *textEngine;
	SDL_Event event;
} Context;

extern Context ctx;


#endif

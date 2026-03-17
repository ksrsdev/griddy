#ifndef GUARD_STATE_RESOURCES_H
#define GUARD_STATE_RESOURCES_H

#include <SDL3/SDL.h>
#include <SDL3_shadercross/SDL_shadercross.h>

#include "error.h"

//INTRO 
typedef struct {
	TTF_Text *title;
	SDL_Texture *titleTargetTexture;
} IntroResources;

#endif

#ifndef GUARD_STATE_RESOURCES_H
#define GUARD_STATE_RESOURCES_H

#include <SDL3/SDL.h>
#include <SDL3_shadercross/SDL_shadercross.h>

#include "error.h"

//ERROR

typedef struct {
	TTF_Text *textObjects[ERROR_UI_COUNT];
	SDL_Texture *textures[ERROR_UI_COUNT];
} ErrorResources;

//INTRO 
typedef struct {
	TTF_Text *title;
	SDL_Texture *titleTargetTexture;
//	SDL_GPURenderState *gradientState;
} IntroResources;

#endif

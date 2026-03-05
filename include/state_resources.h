#ifndef GUARD_STATE_RESOURCES_H
#define GUARD_STATE_RESOURCES_H

#include <SDL3/SDL.h>

//ERROR
typedef struct {
	TTF_Text *title;
	TTF_Text *errorMsg;
	TTF_Text *okButtonText;
} ErrorResources;

//INTRO 

typedef struct {
	TTF_Text *title;
	SDL_Texture *titleTargetTexture;
} IntroResources;

#endif

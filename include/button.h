#ifndef GUARD_BUTTON_H
#define GUARD_BUTTON_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "context.h"

//TODO probably move this to a ui.h or re-name this file because now all UI elements are the same under the hood
typedef void (*OnClick)(GameData* data);

#endif

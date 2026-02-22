#ifndef GUARD_INTRO_H
#define GUARD_INTRO_H

#include <SDL3/SDL.h>

#include "text.h"

//forward declarations
typedef struct GameEngine GameEngine;
typedef struct GameInput GameInput;
typedef struct GameData GameData;

typedef struct{
//	TextBox title;
	SDL_FRect rect; //NOTE REMOVE SDL INCLUDE WHEN YOU REMOVE THIS!
} Layout_Intro;

void Tick_Intro(const GameInput *input, GameData *data);
void Render_Intro(const GameEngine *eng, const GameData *data);

#endif

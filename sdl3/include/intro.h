#ifndef GUARD_INTRO_H
#define GUARD_INTRO_H

#include <SDL3/SDL.h>

#include "text.h"

//forward declarations
typedef struct GameEngine GameEngine;
typedef struct GameInput GameInput;
typedef struct GameData GameData;

typedef struct{
	TextBox title;
	SDL_FRect rect; //NOTE REMOVE SDL INCLUDE WHEN YOU REMOVE THIS!
	uint64_t startTime;
} Layout_Intro;

void Intro_Init(GameData *data);
void Intro_Tick(const GameInput *input, GameData *data);
void Intro_Render(const GameEngine *eng, const GameData *data);
void Intro_Cleanup(GameData *data);

#endif

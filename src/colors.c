#include "colors.h"

#include <SDL3/SDL.h>

bool Colors_AreEqual(SDL_Color color1, SDL_Color color2)[[unsequenced]] 
{
	if (
			color1.r == color2.r &&
			color1.g == color2.g &&
			color1.b == color2.b &&
			color1.a == color2.a
	   ) {
		return true;
	} else {
		return false;
	}
}

SDL_Color Colors_GetRainbowColor(f32 progress)
{
	f32 stage = progress * 6.0f;

	s32 currStage = (s32)stage % 6;

	f32 mod = stage - SDL_floorf(stage);

	f32 increasing = 255.0f * mod;
	f32 decreasing = 255.0f - (255.0f * mod);

	switch(currStage) {
		case 0:
			return (SDL_Color){255, (u8)increasing, 0, 255};
		case 1:
			return (SDL_Color){(u8)decreasing, 255, 0, 255};
		case 2:
			return (SDL_Color){0, 255, (u8)increasing, 255};
		case 3:
			return (SDL_Color){0, (u8)decreasing, 255, 255};
		case 4:
			return (SDL_Color){(u8)increasing, 0, 255, 255};
		case 5:
			return (SDL_Color){255, 0, (u8)decreasing, 255};
		default:
			return (SDL_Color){255, 0, 0 , 255};
	}
}



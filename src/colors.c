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



#include "text.h"

#include <stdio.h>


SDL_Texture* CreateTextureFromText(SDL_Renderer *renderer, TTF_Text *text)
{
	int textW, textH;
	TTF_GetTextSize(text, &textW, &textH);

	SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, textW, textH);

	//Switch renderer to texture
	SDL_SetRenderTarget(renderer, texture);

	//Clear texture
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	
	//Draw text to texture
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	TTF_DrawRendererText(text, 0, 0);

	//return renderer to window
	SDL_SetRenderTarget(renderer, NULL);

	//Make sure texture scales linearly (support SDF font)
	SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_LINEAR);

	// Set the blend mode on the texture so it supports transparency when drawn later
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	

	return texture;
}

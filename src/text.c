#include "text.h"

#include <stdio.h>

#include "context.h"
#include "types.h"

SDL_Texture* CreateTextureFromText(SDL_Renderer *renderer, TTF_Text *text)
{
	printf("HOLA\n");
	int textW, textH;
	TTF_GetTextSize(text, &textW, &textH);

	SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, textW, textH);
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND); // CRITICAL: Copy raw data, don't blend!
	SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_LINEAR); 

	//Switch renderer to texture
	SDL_SetRenderTarget(renderer, texture);

	//Clear texture
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	
	//Draw text to texture
	u8 r, g, b, a;
	if (!TTF_GetTextColor(text, &r, &g, &b, &a)) {
		printf("Can't find text color!");
		r = 255;
		g = 255;
		b = 255;
		a = 255;
	} else {
		printf("Color: %d, %d, %d, %d\n", r, g, b, a);
	}
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	TTF_DrawRendererText(text, 0, 0);

	//return renderer to window
	SDL_SetRenderTarget(renderer, NULL);

	return texture;
}

SDL_Texture* CreateTextureViaSurfaceFromText(SDL_Renderer *renderer, TTF_Font *font, const char* string)
{
    // 1. Ensure SDF is on
    TTF_SetFontSDF(font, true);

    // 2. Render to a surface first. 
    // This is the ONLY way to get the raw 8-bit distance field data.
	// Use SDL_strlen(string) or just 0 if your version treats 0 as 'auto' 
	// BUT many SDL3 builds require the actual count or a specific flag.
	SDL_Surface *surf = TTF_RenderText_Blended(font, string, SDL_strlen(string), (SDL_Color){255, 255, 255, 255});
    if (!surf) return NULL;

    // 3. Upload that raw data to the GPU
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
    
    // 4. Set blend mode so your shader can handle the transparency
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    SDL_DestroySurface(surf);
    return texture;
}

void Text_SetColor(TTF_Text *text, SDL_Color color)
{
	if (!text) {
		//ERROR
		return;
	}

	TTF_SetTextColor(text, color.r, color.g, color.b, color.a);
}

void Text_DrawCentered(TTF_Text *text, SDL_FRect *destRect)
{
	Vector2 textSize = {0};
	TTF_GetTextSize(text, &textSize.x, &textSize.y);

	FVector2 textPos = {0};
	textPos.x = destRect->x + ((destRect->w - (float)textSize.x) / 2.0f);
	textPos.y = destRect->y + ((destRect->h - (float)textSize.y) / 2.0f);

	TTF_DrawRendererText(text, textPos.x, textPos.y);
}


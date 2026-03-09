#include "text.h"

#include <stdio.h>

#include "context.h"
#include "types.h"

SDL_Texture* CreateTextureFromText(SDL_Renderer *renderer, TTF_Text *text)
{
	int textW, textH;
	TTF_GetTextSize(text, &textW, &textH);

	SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, textW, textH);

	//Switch renderer to texture
	SDL_SetRenderTarget(renderer, texture);

	//Clear texture
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	
	//Draw text to texture
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	TTF_DrawRendererText(text, 0, 0);

	//return renderer to window
	SDL_SetRenderTarget(renderer, NULL);

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

#define FONT_BASE_SMALL 8
#define FONT_BASE_MEDIUM 16
#define FONT_BASE_LARGE 32
#define FONT_BASE_TITLE 64

bool Text_LoadFonts(GameFonts *fonts, const TextureScale textureScale)
{
	float fontScale = 0;
	switch (textureScale) {
		case TEXTURE_SCALE_SMALL:
			fontScale = 1.0;
			break;
		case TEXTURE_SCALE_MEDIUM:
			fontScale = 2.0;
			break;
		case TEXTURE_SCALE_LARGE:
			fontScale = 4.0;
			break;
		case TEXTURE_SCALE_MAX:
			fontScale = 8.0;
			break;
		default:
			printf("textureScale OOB\n");
			return false;
	}

	float smallSize  = FONT_BASE_SMALL * fontScale;
	float mediumSize = FONT_BASE_MEDIUM * fontScale;
	float largeSize  = FONT_BASE_LARGE * fontScale;
	float titleSize  = FONT_BASE_TITLE * fontScale;

	fonts->title = TTF_OpenFont("assets/fonts/Press_Start_2P/PressStart2P-Regular.ttf", titleSize);
	if (!fonts->title) {
		printf("Failed to load fonts->title\n");
		return false;
	}
	
	fonts->large = TTF_OpenFont("assets/fonts/Press_Start_2P/PressStart2P-Regular.ttf", largeSize);
	if (!fonts->large) {
		printf("Failed to load fonts->large\n");
		TTF_CloseFont(fonts->title);
		return false;
	}
	
	fonts->medium = TTF_OpenFont("assets/fonts/Press_Start_2P/PressStart2P-Regular.ttf", mediumSize);
	if (!fonts->medium) {
		printf("Failed to load fonts->medium\n");
		TTF_CloseFont(fonts->title);
		TTF_CloseFont(fonts->large);
		return false;
	}
	
	fonts->small = TTF_OpenFont("assets/fonts/Press_Start_2P/PressStart2P-Regular.ttf", smallSize);
	if (!fonts->small) {
		printf("Failed to load fonts->small\n");
		TTF_CloseFont(fonts->title);
		TTF_CloseFont(fonts->large);
		TTF_CloseFont(fonts->medium);
		return false;
	}

	//success
	return true;
}

#undef FONT_BASE_SMALL 
#undef FONT_BASE_MEDIUM
#undef FONT_BASE_LARGE 
#undef FONT_BASE_TITLE 

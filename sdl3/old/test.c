//C Headers
#include <stdio.h>
//SDL Headers
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>

static void DrawTextInBox(TTF_Text *text, SDL_FRect box) {
	int textW, textH;
	TTF_GetTextSize(text, &textW, &textH);
	float posX, posY;
	posX = box.x + ((box.w - (float)textW) / 2);
	posY = box.y + ((box.w - (float)textH) / 2);
	TTF_DrawRendererText(text, posX, posY);
}

int main(void) {
	//Init SDL Video
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("SDL_Init Error: %s", SDL_GetError());
		return 1;
	}
	//Init SDL TTF
	if (!TTF_Init()) {
		SDL_Log("SDL_Init Error: %s", SDL_GetError());
		return 1;
	}
	//Create Window and Renderer
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	//Create the window with renderer
	if (!SDL_CreateWindowAndRenderer("Hello SDL3 Box", 960, 540, 0, &window, &renderer)) {
        SDL_Log("Failed to create window/renderer: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }
	//Set render speed to match VSync (not max out CPU speed match monitor so like 60/120/420 hz)
	SDL_SetRenderVSync(renderer, 1);
	//Set window resizable
	if (!SDL_SetWindowResizable(window, true)) {
		SDL_Log("Failed to set window resizable: %s", SDL_GetError());
		SDL_Quit();
		return 1;
	}
	if (!SDL_SetRenderLogicalPresentation(renderer, 1920, 1080, SDL_LOGICAL_PRESENTATION_LETTERBOX)) {
        SDL_Log("Failed to set Renderer Logical Size: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }
	//Text variables:
	//Create Text Engine (SDL3 supercedes surface -> texture pipeline)
	TTF_TextEngine *engine = TTF_CreateRendererTextEngine(renderer);
	//load font
	TTF_Font *font = TTF_OpenFont("fonts/Press_Start_2P/PressStart2P-Regular.ttf", 16);
	//test text 
	TTF_Text *helloText = TTF_CreateText(engine, font, "Hello World", 0);
	//text color
	TTF_SetTextColor(helloText, 45, 45, 90, 255);

	//Big Bang stuff
	TTF_Font *bangFont = TTF_OpenFont("fonts/Press_Start_2P/PressStart2P-Regular.ttf", 128);
	TTF_Text *bangText = TTF_CreateText(engine, bangFont, "TEST!", 0);
	SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1920, 1080);
	//The Loop
	bool quit = false;
	SDL_Event event;
	while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                quit = true;
            }
        }
       	//Clear window 
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
		//Game Area
		SDL_FRect gameBackground = { 0, 0, 1920, 1080 };
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Dark Grey
		SDL_RenderFillRect(renderer, &gameBackground);
        // Draw a Red Rectangle
        SDL_FRect rect = { 220, 140, 200, 200 }; // x, y, width, height
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
		if (!TTF_DrawRendererText(helloText, 100.0f, 100.0f)) {
			SDL_Log("Shadow realm! Drawing failed: %s", SDL_GetError());
		}
		DrawTextInBox(helloText, rect);
		//Switch renderer target to texture
		SDL_SetRenderTarget(renderer, texture);
		//clear the texture
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
		//
		TTF_DrawRendererText(bangText, 0, 0);
		SDL_SetRenderTarget(renderer, NULL);
		SDL_FRect bigBangRect = { 960, 540, 192, 108};
		SDL_RenderTextureRotated (renderer, texture, NULL, &bigBangRect, 30, NULL, SDL_FLIP_NONE);
        // Present the backbuffer (Show what we drew)
        SDL_RenderPresent(renderer);
    }
	//Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
	//Text cleanup
	TTF_DestroyRendererTextEngine(engine);
	TTF_DestroyText(helloText);
	TTF_CloseFont(font);
	TTF_DestroyText(bangText);
	TTF_CloseFont(bangFont);
	TTF_Quit();
	//Texture Cleanup
	SDL_DestroyTexture(texture);
	//quit
    SDL_Quit();
    return 0;
}

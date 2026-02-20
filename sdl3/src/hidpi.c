//C Headers
#include <stdio.h>
//SDL Headers
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>

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
	//Create window and renderer
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	SDL_CreateWindowAndRenderer("HiDPI Test", 960, 540, SDL_WINDOW_HIGH_PIXEL_DENSITY, &window, &renderer);
	//Set window resizable
	SDL_SetWindowResizable(window, true);
	//normalize screen size
    SDL_SetRenderLogicalPresentation(renderer, 1920, 1080, SDL_LOGICAL_PRESENTATION_LETTERBOX);
	//throttle cpu
	SDL_SetRenderVSync(renderer, 1);
	//Declare Text variables
	TTF_TextEngine *engine = TTF_CreateRendererTextEngine(renderer);
	TTF_Font *font = TTF_OpenFont("fonts/Press_Start_2P/PressStart2P-Regular.ttf", 16);
	TTF_Text *text = TTF_CreateText(engine, font, "TEST!", 0);
	TTF_SetTextColor(text, 0, 0, 0, 255);
	float scale = SDL_GetWindowDisplayScale(window); 
	printf("window display scale: %f\n", (double)scale);
	float renderScaleX, renderScaleY;
	SDL_GetRenderScale(renderer, &renderScaleX, &renderScaleY);
	printf("render scale: %f, %f\n", (double)renderScaleX, (double)renderScaleY);
	int winW, winH;
	int pixelW, pixelH;

	SDL_GetWindowSize(window, &winW, &winH);
	SDL_GetRenderOutputSize(renderer, &pixelW, &pixelH);

	float pixelDensity = (float)pixelW / (float)winW;
	printf("Pixel Density: %f\n", (double)pixelDensity);
// This usually gives you the ratio of physical pixels to logical points
	int textW, textH;
	TTF_GetTextSize(text, &textW, &textH);
	SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, textW, textH);
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
	bool quit = false;
	TTF_GetTextSize(text, &textW, &textH);
	SDL_FRect textureRect = {200, 200, (float)textW, (float)textH};
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

		//Draw text from engine direct
		TTF_DrawRendererText(text, 100.0f, 100.0f);
	
		//Switch target to texture
		SDL_SetRenderTarget(renderer, texture);
		//clear texture 
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
		//render text to texture
		TTF_DrawRendererText(text, 0, 0);
		//return renderer to window
		SDL_SetRenderTarget(renderer, NULL);
		//render the actual texuture
		SDL_RenderTexture (renderer, texture, NULL, &textureRect);

       	//Present the rendered stuff 
		SDL_RenderPresent(renderer);
	}
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
	//Text cleanup
	TTF_DestroyRendererTextEngine(engine);
	TTF_DestroyText(text);
	TTF_CloseFont(font);
	TTF_Quit();
    SDL_Quit();
    return 0;
}

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

int main(int argc, char *argv[]) {
	(void)argc;
	(void)argv;
	//Init SDL Video
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("SDL_Init Error: %s", SDL_GetError());
		return 1;
	}
	
	//Create Window and Renderer
	
	//Declare window and renderer pointer vars
	//NOTE: SDL defaults to linear
//	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"); 
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	//Create the window with renderer
	if (!SDL_CreateWindowAndRenderer("Hello SDL3 Box", 960, 540, 0, &window, &renderer)) {
        SDL_Log("Failed to create window/renderer: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }
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
		SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255); // Dark Grey
		SDL_RenderFillRect(renderer, &gameBackground);
        // Draw a Red Rectangle
        SDL_FRect rect = { 220, 140, 200, 200 }; // x, y, width, height
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
        // Present the backbuffer (Show what we drew)
        SDL_RenderPresent(renderer);
    }

	//Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

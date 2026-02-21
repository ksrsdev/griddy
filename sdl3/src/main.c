#include <stdio.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "context.h"

Context ctx = {
	.shouldQuit = false,
	.window = NULL,
	.renderer = NULL,
	.textEngine = NULL,
	.event = {0},
};

static int InitSDLSubsystems(void)
{
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
	return 0;
}

static void CleanupContextStruct(void)
{
	SDL_DestroyRenderer(ctx.renderer);
	SDL_DestroyWindow(ctx.window);
	TTF_DestroyRendererTextEngine(ctx.textEngine);
}

static void ReportCurrentScales(SDL_Renderer *renderer, SDL_Window *window) {
	//Display and render scale check:
	float scale = SDL_GetWindowDisplayScale(window); 
	printf("window display scale: %f\n", (double)scale);
	float renderScaleX, renderScaleY;
	SDL_GetRenderScale(renderer, &renderScaleX, &renderScaleY);
	printf("render scale: %f, %f\n", (double)renderScaleX, (double)renderScaleY);
	int winW, winH;
	SDL_GetWindowSize(window, &winW, &winH);
	printf("window X: %f\nwindow Y: %f\n", (double)winW, (double)winH);
	int pixelW, pixelH;
	SDL_GetRenderOutputSize(renderer, &pixelW, &pixelH);
	float pixelDensity = (float)pixelW / (float)winW;
	printf("Pixel Density: %f\n", (double)pixelDensity);
}

int main(void)
{
	if (InitSDLSubsystems() == 1) {
		SDL_Quit();
		return 1;
	}
	//Create window and renderer
	SDL_SetHint(SDL_HINT_VIDEO_WAYLAND_ALLOW_LIBDECOR, "0"); 
	SDL_CreateWindowAndRenderer("Hello SDL!", 960, 540, SDL_WINDOW_HIGH_PIXEL_DENSITY, &ctx.window, &ctx.renderer);
	//Set window resizable
	SDL_SetWindowResizable(ctx.window, true);
	//throttle cpu
	SDL_SetRenderVSync(ctx.renderer, 1);
	ctx.textEngine = TTF_CreateRendererTextEngine(ctx.renderer);
	//Loop
	while (!ctx.shouldQuit) {
		//Input
        while (SDL_PollEvent(&ctx.event)) {
            if (ctx.event.type == SDL_EVENT_QUIT) {
                ctx.shouldQuit = true;
            }
			if (ctx.event.type == SDL_EVENT_WINDOW_RESIZED) {
				printf("WINDOW RESIZED!\n");
				ReportCurrentScales(ctx.renderer, ctx.window);
			}
			if (ctx.event.type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED) {
				printf("PIXEL SIZE CHANGED!\n");
				ReportCurrentScales(ctx.renderer, ctx.window);
			}
        }
		//Logic

		//Draw
		SDL_RenderPresent(ctx.renderer); //This is the final step. Always

		//Here you would want to 'reset' the click vars

	}
	CleanupContextStruct();
	TTF_Quit();
	SDL_Quit();
	return 0;
}

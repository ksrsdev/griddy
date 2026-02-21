#include <stdio.h>

#include <SDL3/SDL.h>

#include "context.h"
#include "input.h"

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

static void ClearInput (GameInput *input)
{
	input->mouseButtonPressed = false;
	input->windowResized = false;
	input->newWindowWidth = 0;
	input->newWindowHeight = 0;
}

void Input_PollEvents(GameEngine *eng, GameInput *input)
{
	ClearInput(input);
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_EVENT_QUIT) {
			input->quitRequested = true;
		}
		if (event.type == SDL_EVENT_WINDOW_RESIZED) {
			printf("WINDOW RESIZED!\n");
			ReportCurrentScales(eng->renderer, eng->window);
		}
		if (event.type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED) {
			printf("PIXEL SIZE CHANGED!\n");
			ReportCurrentScales(eng->renderer, eng->window);
		}
	}
}

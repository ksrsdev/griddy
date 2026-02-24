#include "input.h"

#include <stdio.h>

#include <SDL3/SDL.h>

#include "context.h"

//   ***   static FUNCTION DECLARATIONS   ***  

static void ClearInput (GameInput *input);
static void RecordWindowSize(GameEngine *eng, GameInput *input);
static void ReportCurrentScales(SDL_Renderer *renderer, SDL_Window *window);

//   ***   FUNCTION DEFINITIONS   ***  

void InputPollEvents(GameEngine *eng, GameInput *input)
{
	ClearInput(input);

	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_EVENT_QUIT) {
			input->quitRequested = true;
		}
		if (event.type == SDL_EVENT_WINDOW_RESIZED) {
			RecordWindowSize(eng, input);
		}
		if (event.type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED) {
			printf("PIXEL SIZE CHANGED!\n");
			ReportCurrentScales(eng->renderer, eng->window);
		}
	}
}

static void ClearInput (GameInput *input)
{
	input->mouseButtonPressed = false;
	input->windowResized = false;
	input->newWindowSize = (Vector){0};
}

static void RecordWindowSize(GameEngine *eng, GameInput *input) 
{
	input->windowResized = true;
	int winW, winH;
	SDL_GetWindowSize(eng->window, &winW, &winH);
	input->newWindowSize.x = winW;
	input->newWindowSize.y = winH;
}

static void ReportCurrentScales(SDL_Renderer *renderer, SDL_Window *window)
{
	//Display and render scale check:
	float scale = SDL_GetWindowDisplayScale(window); 
	printf("window display scale: %f\n", (double)scale);

	//Render Scale
	float renderScaleX, renderScaleY;
	SDL_GetRenderScale(renderer, &renderScaleX, &renderScaleY);
	printf("render scale: %f, %f\n", (double)renderScaleX, (double)renderScaleY);

	//Window Size
	int winW, winH;
	SDL_GetWindowSize(window, &winW, &winH);
	printf("window X: %f\nwindow Y: %f\n", (double)winW, (double)winH);

	//Pixel Density
	int pixelW, pixelH;
	SDL_GetRenderOutputSize(renderer, &pixelW, &pixelH);
	float pixelDensity = (float)pixelW / (float)winW;
	printf("Pixel Density: %f\n", (double)pixelDensity);
}


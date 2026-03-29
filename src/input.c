#include "input.h"

#include <stdio.h>

#include <SDL3/SDL.h>

#include "context.h"

#define MOUSE_BUTTON_DIR_DOWN 1
#define MOUSE_BUTTON_DIR_UP   2

//   ***   static FUNCTION DECLARATIONS   ***  

static void ClearInput (GameInput *input);
static void RecordWindowSize(const GameEngine *eng, GameInput *input);
static void RecordMouseMotion(GameInput *input);
static void RecordMouseButton(const SDL_MouseButtonEvent *buttonEvent, const int dir, GameInput *input);
//static void ReportCurrentScales(SDL_Renderer *renderer, SDL_Window *window);

//   ***   FUNCTION DEFINITIONS   ***  

void Input_PollEvents(const GameEngine *eng, GameInput *input)
{
	ClearInput(input);

	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_EVENT_QUIT) {
			input->quitRequested = true;
			return;
		}
		
		if (event.type == SDL_EVENT_WINDOW_RESIZED) {
			RecordWindowSize(eng, input);
		}

		if (event.type == SDL_EVENT_MOUSE_MOTION) {
			RecordMouseMotion(input);
		}

		if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
			RecordMouseButton(&event.button, MOUSE_BUTTON_DIR_DOWN, input);
		}

		if (event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
			RecordMouseButton(&event.button, MOUSE_BUTTON_DIR_UP, input);
		}




		//if (event.type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED) {
		//	printf("PIXEL SIZE CHANGED!\n");
		//	ReportCurrentScales(eng->renderer, eng->window);
		//}
	}
}

static void ClearInput (GameInput *input)
{
	input->mouse.left.wasPressed = false;
	input->mouse.left.wasReleased = false;
	input->mouse.right.wasPressed = false;
	input->mouse.right.wasReleased = false;
	input->mouse.middle.wasPressed = false;
	input->mouse.middle.wasReleased = false;
	input->mouse.moved = false;
	input->window.resized = false;
}

static void RecordWindowSize(const GameEngine *eng, GameInput *input) 
{
	input->window.resized = true;
	int winW, winH;
	SDL_GetWindowSize(eng->window, &winW, &winH);
	input->window.size.x = winW;
	input->window.size.y = winH;
	input->window.timeResized = SDL_GetTicks();
}

static void RecordMouseMotion(GameInput *input)
{
	input->mouse.moved = true;
	float mX, mY;
	SDL_GetMouseState(&mX, &mY);
	input->mouse.pos.x = mX;
	input->mouse.pos.y = mY;
}

static void RecordMouseButton(const SDL_MouseButtonEvent *buttonEvent, const int dir, GameInput *input)
{
	ButtonState *buttonState = nullptr;
	switch (buttonEvent->button) {
		case SDL_BUTTON_LEFT:
			buttonState = &input->mouse.left;
			break;
		case SDL_BUTTON_MIDDLE:
			buttonState = &input->mouse.middle;
			break;
		case SDL_BUTTON_RIGHT:
			buttonState = &input->mouse.right;
			break;
		default:
			//ERROR
			return;
	}
	if (dir == MOUSE_BUTTON_DIR_DOWN) {
		buttonState->isDown = true;
		buttonState->wasPressed = true;
	} else if (dir == MOUSE_BUTTON_DIR_UP) {
		buttonState->isDown = false;
		buttonState->wasReleased = true;
	} else {
		//ERROR
		return;
	}
}

//static void ReportCurrentScales(SDL_Renderer *renderer, SDL_Window *window)
//{
//	//Display and render scale check:
//	float scale = SDL_GetWindowDisplayScale(window); 
//	printf("window display scale: %f\n", (double)scale);
//
//	//Render Scale
//	float renderScaleX, renderScaleY;
//	SDL_GetRenderScale(renderer, &renderScaleX, &renderScaleY);
//	printf("render scale: %f, %f\n", (double)renderScaleX, (double)renderScaleY);
//
//	//Window Size
//	int winW, winH;
//	SDL_GetWindowSize(window, &winW, &winH);
//	printf("window X: %f\nwindow Y: %f\n", (double)winW, (double)winH);
//
//	//Pixel Density
//	int pixelW, pixelH;
//	SDL_GetRenderOutputSize(renderer, &pixelW, &pixelH);
//	float pixelDensity = (float)pixelW / (float)winW;
//	printf("Pixel Density: %f\n", (double)pixelDensity);
//}
//

#undef MOUSE_BUTTON_DIR_DOWN
#undef MOUSE_BUTTON_DIR_UP


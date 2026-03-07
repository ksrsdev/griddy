#include "sync_input.h"


#include "context.h"
#include "types.h"

static void ResetGameDataInputBools(WindowState *window, MouseState *mouse);
static void SyncWindowSize(const Vector2 newWindowSize, WindowState *window);
static void SyncMousePos(const FVector2 newMousePos, MouseState *mouse);
static void SyncButton(const ButtonState newState, ButtonState *currState);


void Main_SyncInput(const GameInput *input, GameData *data)
{
	//Handle Quit Request ASAP
	data->isRunning = !input->quitRequested;
	if (!data->isRunning) {
		return;
	}

	ResetGameDataInputBools(&data->window, &data->mouse);	

	
	//Handle Resize
	if (input->window.resized) {
		SyncWindowSize(input->window.size, &data->window);
		data->window.resized = true;
	}

	//Handle Mouse
	if (input->mouse.moved == true) {
		SyncMousePos(input->mouse.pos, &data->mouse);
		data->mouse.moved = true;
		data->mouse.pos.x = input->mouse.pos.x;
		data->mouse.pos.y = input->mouse.pos.y;
	}

	//Left button
	if (input->mouse.left.wasPressed || input->mouse.left.wasReleased) {
		SyncButton(input->mouse.left, &data->mouse.left);
	}
	
	//Middle button
	if (input->mouse.left.wasPressed || input->mouse.left.wasReleased) {
		SyncButton(input->mouse.left, &data->mouse.left);
	}
	
	//Right button
	if (input->mouse.left.wasPressed || input->mouse.left.wasReleased) {
		SyncButton(input->mouse.left, &data->mouse.left);
	}
	
}

//Default false. Set true during check inputs phase if needed
static void ResetGameDataInputBools(WindowState *window, MouseState *mouse)
{
	window->resized = false;

	mouse->moved = false;
	mouse->left.wasPressed = false;
	mouse->left.wasReleased = false;
	mouse->right.wasPressed = false;
	mouse->right.wasReleased = false;
	mouse->middle.wasPressed = false;
	mouse->middle.wasReleased = false;
}

static void SyncWindowSize(const Vector2 newWindowSize, WindowState *window)
{
	window->resized = true;
	window->size.x = newWindowSize.x;
	window->size.y = newWindowSize.y;
}

static void SyncMousePos(const FVector2 newMousePos, MouseState *mouse)
{
	mouse->moved = true;
	mouse->pos.x = newMousePos.x;
	mouse->pos.y = newMousePos.y;
}

static void SyncButton(const ButtonState newState, ButtonState *currState)
{
	currState->isDown = newState->isDown;
	currState->wasPressed = newState->isDown;
	currState->wasReleased = newState->wasReleased;
}

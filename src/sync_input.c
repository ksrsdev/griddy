#include "sync_input.h"


#include "context.h"
#include "types.h"

static void ResetGameDataInputBools(WindowState *window, MouseState *mouse);
static void ResetMouse(MouseState *mouse);

static void SyncWindow(const WindowState *newState, WindowState *currState);
static void SyncWindowSize(const Vector2 newWindowSize, WindowState *window);

static void SyncMouse(const MouseState *newMouse, MouseState *currMouse);
static void SyncMousePos(const FVector2 newMousePos, MouseState *currMouse);
static void SyncButton(const ButtonState newState, ButtonState *currState);


void Main_SyncInput(const GameInput *input, GameData *data)
{
	//Handle Quit ASAP
	data->isRunning = !input->quitRequested;
	if (!data->isRunning) {
		return;
	}

	ResetGameDataInputBools(&data->window, &data->mouse);	

	SyncWindow(&input->window, &data->window);

	SyncMouse(&input->mouse, &data->mouse);

}

static void ResetGameDataInputBools(WindowState *window, MouseState *mouse)
{
	window->resized = false;

	ResetMouse(mouse);
}

static void ResetMouse(MouseState *mouse)
{
	mouse->moved = false;
	mouse->left.wasPressed = false;
	mouse->left.wasReleased = false;
	mouse->right.wasPressed = false;
	mouse->right.wasReleased = false;
	mouse->middle.wasPressed = false;
	mouse->middle.wasReleased = false;
}

static void SyncWindow(const WindowState *newState, WindowState *currState)
{
	if (!newState->resized) {
		return;
	}
	currState->resized = true;
	SyncWindowSize(newState->size, currState);
}

static void SyncWindowSize(const Vector2 newWindowSize, WindowState *window)
{
	window->resized = true;
	window->size.x = newWindowSize.x;
	window->size.y = newWindowSize.y;
}


static void SyncMouse(const MouseState *newMouse, MouseState *currMouse)
{
	if (newMouse->moved == true) {
		SyncMousePos(newMouse->pos, currMouse);
	}

	//Left button
	if (newMouse->left.wasPressed || newMouse->left.wasReleased) {
//Default false. Set true during check inputs phase if needed
		SyncButton(newMouse->left, &currMouse->left);
	}
	
	//Middle button
	if (newMouse->left.wasPressed || newMouse->left.wasReleased) {
		SyncButton(newMouse->left, &currMouse->left);
	}
	
	//Right button
	if (newMouse->left.wasPressed || newMouse->left.wasReleased) {
		SyncButton(newMouse->left, &currMouse->left);
	}

}

static void SyncMousePos(const FVector2 newMousePos, MouseState *currMouse)
{
	currMouse->moved = true;
	currMouse->pos.x = newMousePos.x;
	currMouse->pos.y = newMousePos.y;
}

static void SyncButton(const ButtonState newState, ButtonState *currState)
{
	currState->isDown = newState.isDown;
	currState->wasPressed = newState.isDown;
	currState->wasReleased = newState.wasReleased;
}

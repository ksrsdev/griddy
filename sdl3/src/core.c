#include "core.h"

//Master Logic Brain
void Core_Tick(const GameInput *input, GameData *data)
{
	//Handle Quit request
	if (input->quitRequested) {
		data->isRunning = false;
	}

}

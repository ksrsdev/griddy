#include "global.h"
#include "state_manager.h"

void StateManager_UpdateGameState(GameState newState) {
	if (newState <= MAIN_GAME_STATE_NONE || newState >= MAIN_GAME_STATE_COUNT) {
		ctx.state = MAIN_GAME_STATE_ERROR;
	}
	ctx.prevState = ctx.state;
	ctx.state = newState;
}

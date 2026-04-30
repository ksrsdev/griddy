#include "play_calling.h"

#include <stdlib.h>

#include "render.h"

void PlayCalling_Init(GameEngine *eng, GameData *data)
{
//	MatchCtx *matchCtx = data->stateData;
//
//	matchCtx->matchStateData = calloc(1, sizeof(PlayCallingData));

	(void)data;
	(void)eng;

	//Init Play Calling UI (scoreboard, buttons)
}

void PlayCalling_Cleanup(GameEngine *eng, GameData *data)
{
	(void)eng;
	(void)data;
}

void PlayCalling_Update(GameData *data)
{
	(void)data;
}

void PlayCalling_Render(const GameEngine *eng, const GameData *data)
{
	(void)data;
	Render_ClearWhite(eng->renderer);
}

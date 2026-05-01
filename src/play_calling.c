
//Six options on offense and defense
//Off: Run, Short Pass, Long Pass, Kneel, Kick, Punt
//Def: Base, Man, Cover, Prevent, Goaline, Blitz

//Run: Low fumble %, likely to gain low yards
//Short Pass: Standard fumble %, less likely than run but better yardage
//Long Pass: high int %, low likelyhood of success but results in a large play
//Kneel: Lose a yard and increase a down - only fails on two successive 0 rolls (1 in 1000)
//Kick: Attempt a field goal. % changes based on distance to goal
//Punt: Punt the ball - 1 % chance he fumbles the punt - another 3% chance the recovering team fumbles - calc result seperate

//Base:    0RN 0SP 0LP 
//Man:     +RN +SP -LP
//Cover:   -RN +SP +LP
//Prevent: -RN 0SP +LP
//Goaline  +RN -SP -LP
//Blitz    ?RN -SP ?LP

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

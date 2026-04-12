#ifndef GUARD_MATCH_H
#define GUARD_MATCH_H

#include "context.h"

typedef enum {
	MATCH_STATE_NONE,

	MATCH_STATE_COIN_TOSS,
	MATCH_STATE_PLAY_CALLING,
	MATCH_STATE_SUMMARY,

	MATCH_STATE_COUNT
} MatchState;

typedef enum {
	POSSESSION_NONE,
	POSSESSION_PLAYER,
	POSSESSION_CPU,
	POSSESSION_COUNT
} MatchPossession;

typedef struct {
	MatchState curr;
	MatchState next;
} MatchStateContext;


//match ctx holds data needed by multiple match states and the void pointer which points to the match state specific data (UI, coin selection, etc)
typedef struct {
	//match state
	MatchStateContext state;

	//vars needed by multiple states
	s32 playerScore;
	s32 cpuScore;
	MatchPossession possesion;

	//pointer to match state specific data
	void *matchStateData;

	//NOT MVP but these would live here as they're needed by multiple match states
	//s32 playerTotalYards;
	//s32 playerFirstDowns;
	//s32 playerTouchDowns;
	//s32 playerCompletedPass;
	//s32 playerFieldGoals;

	//s32 cpuFirstDowns;
	//s32 cpuTotalYards;
	//s32 cpuTouchDowns;
	//s32 cpuCompletedPass;
	//s32 cpuFieldGoals;
} MatchCtx;

void Match_Init(GameEngine *eng, GameData *data);
void Match_Cleanup(GameEngine *eng, GameData *data);
void Match_Update(GameData *data);
void Match_Render(const GameEngine *eng, const GameData *data);


#endif

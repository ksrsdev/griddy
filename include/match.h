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

typedef struct {
	s32 playsRemaining;
	
	s32 playerScore;
	s32 cpuScore;

	s32 lineOfScrimmage;
	s32 down;
	s32 yardsToGain;

	s32 playerPlay;
	s32 cpuPlay;

	s32 possesion;

	//NOT MVP
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
} MatchInfo;

constexpr s32 TOTAL_NUM_PLAYS = 32;

void Match_Init(GameEngine *eng, GameData *data);
void Match_Cleanup(GameEngine *eng, GameData *data);
void Match_Update(GameData *data);
void Match_Render(const GameEngine *eng, const GameData *data);


#endif

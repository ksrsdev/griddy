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

typedef struct {
	s32 playerScore;
	s32 cpuScore;
	MatchPossession pos;
} MatchSession;

//match ctx holds data needed by multiple match states and the void pointer which points to the match state specific data (UI, coin selection, etc)
typedef struct {
	//match state
	MatchStateContext state;

	//vars needed by multiple states
	MatchSession session;

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

//This doesn't really belong here but I put it here because play_sim, play_calling, and scoreboard were all depending on eachother and this file doesn't need anything except context.h - 260504
typedef struct {
	s32 yardsGained;
	s32 points;
	bool playerScore;
	bool offScore;
	bool defScore
	bool turnover;
	bool touchdown;
	bool interception;
	bool score;
	bool firstDown;
	bool fieldGoal;
} PlayResult;

void Match_Init(GameEngine *eng, GameData *data);
void Match_Cleanup(GameEngine *eng, GameData *data);
void Match_Update(GameData *data);
void Match_PostUpdate(GameEngine *eng, GameData *data);
void Match_Render(const GameEngine *eng, const GameData *data);


#endif

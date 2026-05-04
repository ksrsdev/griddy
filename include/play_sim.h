#ifndef GUARD_PLAY_SIM_H
#define GUARD_PLAY_SIM_H

#include "play_calling.h"
#include "scoreboard.h"
#include "types.h"


typedef struct {
	s32 yardsGained;
	s32 points;
	bool playerScore;
	bool turnover;
	bool touchdown;
	bool interception;
	bool score;
	bool firstDown;
	bool fieldGoal;
} PlayResult;

typedef struct {
	PlayID off;
	PlayID def;
} PlayMatchup;

PlayResult PlaySim_Main(const ScoreboardData *sbData, const PlayMatchup plays);

#endif

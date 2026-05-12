#ifndef GUARD_PLAY_SIM_H
#define GUARD_PLAY_SIM_H

#include "play_calling.h"
#include "scoreboard.h"
#include "types.h"

typedef struct {
	PlayID off;
	PlayID def;
} PlayMatchup;

typedef struct {
	s32 netYards;    //positive or negative from the LoS
	s32 offYards;
	s32 defYards;
	u8 pointsScored; //0, 2, 3, 6
	bool isTurnover; //Did the defense gain possession?
	bool defScored;  //Was Def the team who scored the points?
} PlayResult;

PlayResult PlaySim_Main(const ScoreboardData *sbData, const PlayMatchup plays);

#endif

#ifndef GUARD_PLAY_SIM_H
#define GUARD_PLAY_SIM_H

#include "play_calling.h"
#include "scoreboard.h"
#include "types.h"

constexpr s32 POINTS_SAFETY     = 2;
constexpr s32 POINTS_FIELD_GOAL = 3;
constexpr s32 POINTS_TOUCHDOWN  = 7;

typedef struct {
	PlayID off;
	PlayID def;
} PlayMatchup;

typedef struct {
	s32 startSpot;
	s32 endSpot;
	PlayScore score;

	//No Fumble in MVP so only turnover in PlaySim can be an int
	bool isInt; 
} PlayResult;

PlayResult PlaySim_Main(const ScoreboardData *sbData, const PlayMatchup plays);

#endif

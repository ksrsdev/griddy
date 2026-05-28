#ifndef GUARD_PLAY_SIM_H
#define GUARD_PLAY_SIM_H

#include "play_calling.h"
#include "scoreboard.h"
#include "types.h"

constexpr s32 POINTS_SAFETY     = 2;
constexpr s32 POINTS_FIELD_GOAL = 3;
constexpr s32 POINTS_TOUCHDOWN  = 7;

//Basically just a vector but I didn't want to say odds.x rather odds.drop
//Have each odds be their base odds like 5% or 7% or whatever then just add them up on the fly to handle the roll
typedef struct {
	s32 interception;
	s32 incompletion;
} CatchOdds;

typedef enum {
	CATCH_RESULT_NONE,

	CATCH_RESULT_COMPLETION,
	CATCH_RESULT_INCOMPLETION,
	CATCH_RESULT_INTERCEPTION,

	CATCH_RESULT_COUNT
} CatchResult;

typedef struct {
	PlayID off;
	PlayID def;
} PlayMatchup;

typedef struct {
	s32 startSpot;
	s32 endSpot;
	s32 intSpot;
	PlayScore score;

	//No Fumble in MVP so only turnover in PlaySim can be an int
	bool isInt; 
	bool isTouchback;
} PlayResult;

PlayResult PlaySim_Main(const ScoreboardData *sbData, const PlayMatchup plays);

#endif

#include "play_sim.h"

#include <stdio.h>
#include <stdlib.h>

#include "play_arrays.h"
#include "play_calling.h"

//Main Helper Func(s)
static PlayResult PlaySim_InitResultObj(const s32 los);

//Main Sub funcs
static bool PlaySim_IsSpecialTeamsPlay(const PlayID play);

static void PlaySim_SpecialTeamsPlay(const ScoreboardData *sbData, const PlayID play, PlayResult *result);
static void PlaySim_StandardPlay(const ScoreboardData *sbData, const PlayMatchup plays, PlayResult *result);

//Resolve Play
static void PlaySim_ResolvePlay(const ScoreboardData *sb, PlayResult *result);
static void PlaySim_ResolvePlay_Score(PlayResult *result, const MatchPossession pos);
//static bool PlaySim_ResolvePlay_CheckScore(const s32 fieldLen, PlayResult *result);
//static bool PlaySim_ResolvePlay_CheckTouchdown(const s32 fieldLen, PlayResult *result);
//static bool PlaySim_ResolvePlay_CheckSafety(const s32 fieldLen, PlayResult *result);
////Resolve Play ?
//NOT MVP
//static void PlaySim_CheckFumble(PlayResult *result);

//Play specific funcs
static void PlaySim_Run(PlayResult *result, const PlayID def, const MatchPossession pos);
static s32 PlaySim_Run_CalcGain(const PlayID def);
static void PlaySim_ShortPass(PlayResult *result, const PlayID def, const MatchPossession pos);
static void PlaySim_LongPass(PlayResult *result, const PlayID def, const MatchPossession pos, const s32 los);
static void PlaySim_Pass(PlayResult *result, const PlayID off, const PlayID def, const MatchPossession pos, const s32 los);
static void PlaySim_Kneel(const ScoreboardData *sbData, PlayResult *result);
static void PlaySim_Kick(PlayResult *result, const s32 los, const MatchPossession pos);
static void PlaySim_Punt(const ScoreboardData *sbData, PlayResult *result);

static void PlaySim_Sack(PlayResult *result, const MatchPossession pos, const PlayID play);
static s32 PlaySim_CalcSackLoss(const PlayID play);

static CatchResult PlaySim_Catch_CalcResult(const PlayID off, const PlayID def, const bool passBackedUp);
static CatchOdds PlaySim_Catch_CalcOdds(const PlayID off, const PlayID def);
static void  PlaySim_Catch_Completion(PlayResult *result, const s32 gain, const MatchPossession pos);
static void  PlaySim_Catch_Incompletion(PlayResult *result, const MatchPossession pos);
static void  PlaySim_Catch_Interception(PlayResult *result, const s32 gain, const MatchPossession pos);

static bool PlaySim_IsInterceptionTouchdown(const s32 endSpot, const MatchPossession pos);

static void PlaySim_ApplyGain(PlayResult *result, const s32 gain, const MatchPossession pos);

static s32 PlaySim_GetFieldLength(const MatchPossession pos, const s32 los);

static constexpr u32 NUM_OFF_PLAYS =  PLAY_OFF_END - PLAY_OFF_START;
static constexpr u32 NUM_DEF_PLAYS =  PLAY_DEF_END - PLAY_DEF_START;

typedef enum {
	PLAY_ADVANTAGE_NONE,
	PLAY_ADVANTAGE_OFF,
	PLAY_ADVANTAGE_DEF,
	PLAY_ADVANTAGE_PREVENT,
	PLAY_ADVANTAGE_CHAOS,
	PLAY_ADVANTAGE_COUNT
} PlayAdvantage;

static const PlayAdvantage sPlayAdvantageTable[NUM_DEF_PLAYS][NUM_OFF_PLAYS] = {

	//Base
	[PLAY_DEF_BASE - PLAY_DEF_START] = { 
		PLAY_ADVANTAGE_NONE, //Run
		PLAY_ADVANTAGE_NONE, //Short Pas
		PLAY_ADVANTAGE_NONE  //Long Pass
	},

	//Man
	[PLAY_DEF_MAN - PLAY_DEF_START] = { 
		PLAY_ADVANTAGE_DEF, //Run
		PLAY_ADVANTAGE_DEF, //Short Pas
		PLAY_ADVANTAGE_OFF  //Long Pass
	},

	//Cover
	[PLAY_DEF_COVER - PLAY_DEF_START] = { 
		PLAY_ADVANTAGE_OFF, //Run
		PLAY_ADVANTAGE_DEF, //Short Pas
		PLAY_ADVANTAGE_DEF  //Long Pass
	},

	//Prevent
	[PLAY_DEF_PREVENT - PLAY_DEF_START] = { 
		PLAY_ADVANTAGE_PREVENT, //Run
		PLAY_ADVANTAGE_PREVENT, //Short Pas
		PLAY_ADVANTAGE_PREVENT  //Long Pass
	},

	//Goal Line
	[PLAY_DEF_GOAL_LINE - PLAY_DEF_START] = { 
		PLAY_ADVANTAGE_DEF, //Run
		PLAY_ADVANTAGE_OFF, //Short Pas
		PLAY_ADVANTAGE_OFF  //Long Pass
	},

	//Blitz
	[PLAY_DEF_BLITZ - PLAY_DEF_START] = { 
		PLAY_ADVANTAGE_CHAOS, //Run
		PLAY_ADVANTAGE_OFF, //Short Pas
		PLAY_ADVANTAGE_CHAOS  //Long Pass
	},
};

//Percent chance of a sack vs this defense - Long Pass has 2x modifier
static const s32 sSackChanceTable[NUM_DEF_PLAYS] = {
	[PLAY_DEF_BASE - PLAY_DEF_START]      = 6,
	[PLAY_DEF_MAN - PLAY_DEF_START]       = 5,
	[PLAY_DEF_COVER - PLAY_DEF_START]     = 4,
	[PLAY_DEF_PREVENT - PLAY_DEF_START]   = 1,
	[PLAY_DEF_GOAL_LINE - PLAY_DEF_START] = 12,
	[PLAY_DEF_BLITZ - PLAY_DEF_START]     = 25,
};

//Distance a pass goes 
static const s32 sShortPassDistTable[NUM_PLAY_OUTCOMES] = {
	 -4,
	 -1,
	  0,
	  3,
	  5,
	  6,
	  7,
	  9,
	 11,
	 13,
};

static const s32 sLongPassDistTable[NUM_PLAY_OUTCOMES] = {
	 14,
	 16,
	 18,
	 21,
	 24,
	 27,
	 32,
	 36,
	 42,
	 51,
};

static const CatchOdds sShortPassCatchTable[NUM_DEF_PLAYS] = {
    [PLAY_DEF_BASE - PLAY_DEF_START]      = {2, 30},  // 68% Complete (Realistic NFL)
    [PLAY_DEF_MAN - PLAY_DEF_START]       = {3, 35},  // 62% Complete
    [PLAY_DEF_COVER - PLAY_DEF_START]     = {4, 32},  // 64% Complete
    [PLAY_DEF_PREVENT - PLAY_DEF_START]   = {1, 15},  // 84% Complete (Soft defense gives up yards)
    [PLAY_DEF_GOAL_LINE - PLAY_DEF_START] = {6, 40},  // 54% Complete (Tight windows)
    [PLAY_DEF_BLITZ - PLAY_DEF_START]     = {2, 20},  // 78% Complete (Beat the blitz)
};

static const CatchOdds sLongPassCatchTable[NUM_DEF_PLAYS] = {
    [PLAY_DEF_BASE - PLAY_DEF_START]      = {4, 50},  // 46% Complete (Volatile deep balls)
    [PLAY_DEF_MAN - PLAY_DEF_START]       = {4, 55},  // 41% Complete
    [PLAY_DEF_COVER - PLAY_DEF_START]     = {6, 52},  // 42% Complete
    [PLAY_DEF_PREVENT - PLAY_DEF_START]   = {10, 65}, // 25% Complete (Lockdown deep coverage)
    [PLAY_DEF_GOAL_LINE - PLAY_DEF_START] = {2, 30},  // 68% Complete (Burnt goal-line defense)
    [PLAY_DEF_BLITZ - PLAY_DEF_START]     = {5, 45},  // 50% Complete
};

//MAIN
PlayResult PlaySim_Main(const ScoreboardData *sbData, const PlayMatchup plays)
{
	printf("\n#####\npos: %d\n offPlay: %d\ndefPlay: %d\n",sbData->session.pos, plays.off, plays.def);
	PlayResult result = PlaySim_InitResultObj(sbData->los);

	if (plays.off == PLAY_OFF_KNEEL) {
		PlaySim_Kneel(sbData, &result);
	} else if (PlaySim_IsSpecialTeamsPlay(plays.off)) {
		PlaySim_SpecialTeamsPlay(sbData, plays.off, &result);
	} else {
		PlaySim_StandardPlay(sbData, plays, &result);
	}

	return result;
}

static PlayResult PlaySim_InitResultObj(const s32 los)
{
	PlayResult result = {};
	result.startSpot = los;
	result.endSpot   = los;

	return result;
}


static bool PlaySim_IsSpecialTeamsPlay(const PlayID play)
{
	if (play >= PLAY_SPECIAL_TEAMS_START && play < PLAY_SPECIAL_TEAMS_END) {
		return true;
	} 

	return false;
}

static void PlaySim_SpecialTeamsPlay(const ScoreboardData *sbData, const PlayID play, PlayResult *result)
{
	const s32 los = sbData->los;
	const MatchPossession pos = sbData->session.pos;

	switch (play) {
		case PLAY_OFF_KICK:
			PlaySim_Kick(result, los, pos);
			break;
		case PLAY_OFF_PUNT:
			PlaySim_Punt(sbData, result);
			break;
		default:
			//ERROR
			printf("playID OOB PlaySim_SpecialTeamsPlay()\n");
			PlaySim_Kneel(sbData, result);
			break;
	}
}

static void PlaySim_StandardPlay(const ScoreboardData *sbData, const PlayMatchup plays, PlayResult *result)
{
	//Shortcut variables
	const MatchPossession pos = sbData->session.pos;
	const s32 los = sbData->los;

	switch (plays.off) {
		case PLAY_OFF_RUN:
			PlaySim_Run(result, plays.def, pos);
			break;
		case PLAY_OFF_SHORT_PASS:
			PlaySim_ShortPass(result, plays.def, pos);
			break;
		case PLAY_OFF_LONG_PASS:
			PlaySim_LongPass(result, plays.def, pos, los);
			break;
		default:
			//ERROR
			printf("playID OOB PlaySim_StandardPlay()\n");
			PlaySim_Kneel(sbData, result);
			break;
	}

	//MVP just check for score and cap yards if score
	PlaySim_ResolvePlay(sbData, result);
}

//Everything that happens when the "forward progress" stops. 
//This gets called after a run, or after a pass + run after pass (which includes an int)
//Only ever called after a standard play - NOT SPECIAL TEAMS
static void PlaySim_ResolvePlay(const ScoreboardData *sb, PlayResult *result)
{
	printf("PlaySim_ResolvePlay()\n");
	//Cap Yards and check score
	if (result->endSpot >= 100) {
		result->endSpot = 100;
		PlaySim_ResolvePlay_Score(result, sb->session.pos);
	} else if (result->endSpot <= 0) {
		result->endSpot = 0;
		PlaySim_ResolvePlay_Score(result, sb->session.pos);
	}

}

static void PlaySim_ResolvePlay_Score(PlayResult *result, const MatchPossession pos)
{
	printf("PlaySim_ResolvePlay_Score() - type: %d", result->score);

	//No score on a touchback - technically the offense can throw an int into their own endzone so let's account for that
	if (result->isTouchback) {
		if (PlaySim_IsInterceptionTouchdown(result->endSpot, pos)) {
			result->isTouchback = false;
			result->score = SCORE_TOUCHDOWN_DEFENSE;
		}
		return;
	}

	//Defensive touchdown
	//Technically this just says "There was an interception that didn't occur inside an endzone and the ball ended in an endzone - it's possible for a -1 YAC after an interception at the 1 yard line to result in a safety
	if (result->isInt) {
		if (PlaySim_IsInterceptionTouchdown(result->endSpot, pos)) {
			result->score = SCORE_TOUCHDOWN_DEFENSE;
		} else {  //Ball ended in an endzone after an int but def did not make a touchdown - that means they were hit in their own (def) endzone. Result of the play is no score - def takes over wherever the catch was made
			result->endSpot = result->intSpot;
		}
		return;
	}

	//Offense maintained possession and ball ended play in an endzone
	if (pos == POSSESSION_PLAYER) {
		if (result->endSpot == 0) {
			result->score = SCORE_SAFETY;
		} else {
			result->score = SCORE_TOUCHDOWN;
		}
	} else {
		if (result->endSpot == 100) {
			result->score = SCORE_SAFETY;
		} else {
			result->score = SCORE_TOUCHDOWN;
		}
	}

}

//static bool PlaySim_ResolvePlay_CheckScore(const ScoreboardData *sb, PlayResult *result)
//{
//	return false;
//}
//
////FIXME
//static bool PlaySim_ResolvePlay_Checkoppenmheiomer truman sceneTouchdown(const s32 fieldLen, PlayResult *result)
//{
//	return false;
//}
//
//static bool PlaySim_ResolvePlay_CheckSafety(const s32 fieldLen, PlayResult *result)
//{
//	return false;
//}

//NOT MVP
//static void PlaySim_CheckFumble(PlayResult *result)
//{
//
//	s32 roll = rand() % 100;
//	
//	//It's a fumble! Check who recovers
//	if (roll < 1) {
//		
//		roll = rand() % 2;
//		if (roll < 1) {
//			//Defense recovers
//			result->isFumble   = true;
//			result->isTurnover = true;
//		
//			//Check yards after recover
//			roll = rand() % NUM_PLAY_OUTCOMES;
//			s32 gain = FUMBLE_DEF_RECOVERY[roll];
//			result->netYards -= gain;
//			result->defYards += gain;
//		}
//	}
//}
//

//This is the "main" Run play func - it doesn't need to do much since run is such a simple play
static void PlaySim_Run(PlayResult *result, const PlayID def, const MatchPossession pos)
{

	s32 gain = PlaySim_Run_CalcGain(def);

	printf("PlaySim_Run gain: %d\n", gain);

	PlaySim_ApplyGain(result, gain, pos);
}

//This func just returns the gain of the run vs the def playoppenmheiomer truman scene
static s32 PlaySim_Run_CalcGain(const PlayID def)
{
	//Figure out which table to roll from based on play advantage
	PlayAdvantage adv = sPlayAdvantageTable[def - PLAY_DEF_START][PLAY_OFF_RUN- PLAY_OFF_START];

	const s32 *probTable = nullptr;

	switch (adv) {
		case PLAY_ADVANTAGE_NONE:
			probTable = RUN_ADVANTAGE_NONE; 
			break;
		case PLAY_ADVANTAGE_OFF:
			probTable = RUN_ADVANTAGE_OFF; 
			break;
		case PLAY_ADVANTAGE_DEF:
			probTable = RUN_ADVANTAGE_DEF; 
			break;
		case PLAY_ADVANTAGE_PREVENT:
			probTable = RUN_ADVANTAGE_PREVENT; 
			break;
		case PLAY_ADVANTAGE_CHAOS:
			probTable = RUN_ADVANTAGE_CHAOS; 
			break;
		default:
			//ERROR
			printf("adv OOB in PlaySim_Run()\n");
			probTable = RUN_ADVANTAGE_NONE;
			break;
	}

	//Roll on that table and record the result
	s32 roll = rand() % NUM_PLAY_OUTCOMES;

	s32 gain = probTable[roll];

	return gain;
}

//Base sack rate is 5% - double everything on LongPass
//
static void PlaySim_ShortPass(PlayResult *result, const PlayID def, const MatchPossession pos)
{
	//LOS doesn't matter for short pass plays, use a sub
	const s32 DEFAULT_LOS = 50;
	PlaySim_Pass(result, PLAY_OFF_SHORT_PASS, def, pos, DEFAULT_LOS);
}

static void PlaySim_LongPass(PlayResult *result, const PlayID def, const MatchPossession pos, const s32 los)
{
	PlaySim_Pass(result, PLAY_OFF_LONG_PASS, def, pos, los);
}

static void PlaySim_Pass(PlayResult *result, const PlayID off, const PlayID def, const MatchPossession pos, const s32 los)
{
	//Sack / Drop Back phase
	s32 sackChance = sSackChanceTable[def];

	s32 roll = rand() % 100;

	//If Sack then the play ends here thus return
	if (roll <= sackChance) {
		PlaySim_Sack(result, pos, off);
		return;
	}

	//Throw Phase - calc distance
	s32 dist = 0;
	roll = rand() % NUM_PLAY_OUTCOMES;

	if (off == PLAY_OFF_SHORT_PASS) {
		dist = sShortPassDistTable[roll];
	} else if (off == PLAY_OFF_LONG_PASS) {
		dist = sLongPassDistTable[roll];
	} else {
		//ERROR
		printf("ERROR! off OOB in PlaySim_Pass()\n");
		return;
	}
	printf("dist: %d\n", dist);

	//Handle backed up Long Pass
	bool passBackedUp = false;
	if (off == PLAY_OFF_LONG_PASS && dist > 20) {
		s32 fieldLen = PlaySim_GetFieldLength(pos, los);
		if (fieldLen < dist) {
			passBackedUp = true;
		}
	}
	
	//Catch vs Drop vs Int
	CatchResult catchResult = PlaySim_Catch_CalcResult(off, def, passBackedUp);

	//Switch on CatchResult type (comp, incomp, int)
	switch (catchResult) {
		case CATCH_RESULT_COMPLETION:
			PlaySim_Catch_Completion(result, dist, pos);
			break;
		case CATCH_RESULT_INCOMPLETION:
			PlaySim_Catch_Completion(result, dist, pos);
			break;
		case CATCH_RESULT_INTERCEPTION:
			PlaySim_Catch_Interception(result, dist, pos);
			break;
		default:
			//ERROR
			PlaySim_Catch_Incompletion(result, pos);
			break;
	}
	
}

//Just lose one yard - thats it
static void PlaySim_Kneel(const ScoreboardData *sbData, PlayResult *result)
{
	PlaySim_ApplyGain(result, -1, sbData->session.pos);
	PlaySim_ResolvePlay(sbData, result);
}

static void PlaySim_Kick(PlayResult *result, const s32 los, const MatchPossession pos)
{
	s32 fieldLen = PlaySim_GetFieldLength(pos, los);
	s32 successRate = 0;

	if (fieldLen < 15) {
		successRate = 98;
	} else if (fieldLen < 25) {
		successRate = 95;
	} else if (fieldLen < 35) {
		successRate = 80;
	} else if (fieldLen < 43) {
		successRate = 65;
	} else if (fieldLen < 46) {
		successRate = 35;
	} else if (fieldLen < 49) {
		successRate = 18;
	} else if (fieldLen < 54) {
		successRate = 5;
	} else {
		successRate = 0;
	}

	s32 roll = rand() % 100;

	if (roll < successRate) {
		result->score = SCORE_FIELD_GOAL;
	} else {
		//NOTE I should seperate INT from Turnover again but MVP this is what PlayCalling_ApplyResult() switches on for a turnover so yeah
		result->isInt = true;
	}
}

static void PlaySim_Punt(const ScoreboardData *sbData, PlayResult *result)
{
	(void) result;
	(void) sbData;
}

//Adds or subtracts yards from startSpot depending on who has possession
static void PlaySim_ApplyGain(PlayResult *result, const s32 gain, const MatchPossession pos)
{
	if (pos == POSSESSION_PLAYER) {
		result->endSpot = result->startSpot + gain;
	} else {
		result->endSpot = result->startSpot - gain;
	}

	printf("ApplyGain start: %d end: %d\n", result->startSpot, result->endSpot);
}

static void PlaySim_Sack(PlayResult *result, const MatchPossession pos, const PlayID play)
{
	s32 gain = PlaySim_CalcSackLoss(play);

	PlaySim_ApplyGain(result, gain, pos);

}

static s32 PlaySim_CalcSackLoss(const PlayID play)
{
	
	const s32 *sackTable = nullptr;

	if (play == PLAY_OFF_SHORT_PASS) {
		sackTable = SACK_TABLE_SHORT_PASS;
	} else if (play == PLAY_OFF_LONG_PASS) {
		sackTable = SACK_TABLE_LONG_PASS;
	} else {
		//ERROR
		printf("ERROR! play OOB PlaySim_ApplySack()\n");
		sackTable = SACK_TABLE_SHORT_PASS;
	}

	s32 roll = rand() % NUM_PLAY_OUTCOMES;

	s32 gain = sackTable[roll];

	return gain;
}


static void  PlaySim_Catch_Completion(PlayResult *result, const s32 gain, const MatchPossession pos)
{
	//Check yards after catch
	s32 roll = rand() % NUM_PLAY_OUTCOMES;

	s32 yac = RUN_AFTER_CATCH[roll];

	s32 netGain = gain + yac;

	PlaySim_ApplyGain(result, netGain, pos);
}

static void  PlaySim_Catch_Incompletion(PlayResult *result, const MatchPossession pos)
{
	PlaySim_ApplyGain(result, 0, pos);
}

static void  PlaySim_Catch_Interception(PlayResult *result, const s32 gain, const MatchPossession pos)
{
	result->isInt = true;

	//Check touchback
	PlaySim_ApplyGain(result, gain, pos);
	
	if (result->endSpot >= 100) { 
		result->endSpot = 100;
		result->isTouchback = true;
	} else if (result->endSpot <= 0) {
		result->endSpot = 0;
		result->isTouchback = true;
	} 

	result->intSpot = result->endSpot;

	//No run after catch on a touchback - PlayCalling_ApplyResult() handles the update to LoS
	//Also technically I think this /could/ be a safety too so should check for that
	if (result->isTouchback) {
		return;
	}

	//Yards after catch
	
	s32 roll = rand() % NUM_PLAY_OUTCOMES;

	s32 yac = RUN_AFTER_CATCH[roll];

	//This needs to be the original gain - the yards after interception
	s32 netGain = gain - yac;

	MatchPossession defPos = POSSESSION_NONE;

	if (pos == POSSESSION_PLAYER) {
		defPos = POSSESSION_CPU;
	} else {
		defPos = POSSESSION_PLAYER;
	}
	
	PlaySim_ApplyGain(result, netGain, defPos);
}

static CatchResult PlaySim_Catch_CalcResult(const PlayID off, const PlayID def, const bool passBackedUp)
{
	//Assign CatchOdds;
	CatchOdds odds = PlaySim_Catch_CalcOdds(off, def);

	//Roll on odds table
	s32 roll = rand() % 100;

	if (passBackedUp) {
		odds.interception *= 3;

		odds.incompletion *= 3;
		odds.incompletion /= 2;
	}

	//Return CatchResult
	if (roll < odds.interception) {
		return CATCH_RESULT_INTERCEPTION;
	} else if (roll < odds.interception + odds.incompletion) {
		return CATCH_RESULT_INCOMPLETION;
	} else {
		return CATCH_RESULT_COMPLETION;
	}
}

static CatchOdds PlaySim_Catch_CalcOdds(const PlayID off, const PlayID def)
{
	CatchOdds odds = {};

	if (off == PLAY_OFF_SHORT_PASS) {
		odds = sShortPassCatchTable[def - PLAY_DEF_START];
	} else {
		odds =  sLongPassCatchTable[def - PLAY_DEF_START];
	}

	return odds;
}

//An interception occured where the intSpot was not in the end zone but the ball ended in an endzone - either a defense touchdown or else a "safety" but it's really just the def takes over at the intSpot
static bool PlaySim_IsInterceptionTouchdown(const s32 endSpot, const MatchPossession pos)
{

	//NOTE: Pos is the team which began play with possession ie off

	if (pos == POSSESSION_PLAYER && endSpot == 0) {
		return true;
	} else if (pos == POSSESSION_CPU && endSpot == 100) {
		return true;
	} else {
		return false;
	}
}

static s32 PlaySim_GetFieldLength(const MatchPossession pos, const s32 los)
{
	s32 fieldLen = 0;
	if (pos == POSSESSION_PLAYER) {
		fieldLen = 100 - los;
	} else {
		fieldLen = los;
	}

	return fieldLen;
}




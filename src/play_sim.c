#include "play_sim.h"

#include <stdio.h>
#include <stdlib.h>

#include "play_arrays.h"
#include "play_calling.h"

//Main Sub funcs
static bool PlaySim_IsSpecialTeamsPlay(const PlayID play);
static void PlaySim_SpecialTeamsPlay(const ScoreboardData *sbData, const PlayID play, PlayResult *result);
static void PlaySim_StandardPlay(const ScoreboardData *sbData, const PlayMatchup plays, PlayResult *result);

static void PlaySim_CheckFumble(PlayResult *result);

//Resolve Play
static void PlaySim_ResolvePlay(const ScoreboardData *sb, PlayResult *result);
static bool PlaySim_ResolvePlay_CheckTouchdown(const s32 fieldLen, PlayResult *result);
static bool PlaySim_ResolvePlay_CheckSafety(const s32 fieldLen, PlayResult *result);

//Play specific funcs
static void PlaySim_Run(PlayResult *result, const PlayID def, const s32 los);
static void PlaySim_ShortPass(const ScoreboardData *sbData, PlayResult *result);
static void PlaySim_LongPass(const ScoreboardData *sbData, PlayResult *result);
static void PlaySim_Kneel(const ScoreboardData *sbData, PlayResult *result);
static void PlaySim_Kick(const ScoreboardData *sbData, PlayResult *result);
static void PlaySim_Punt(const ScoreboardData *sbData, PlayResult *result);

//Helper funcs
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

//MAIN
PlayResult PlaySim_Main(const ScoreboardData *sbData, const PlayMatchup plays)
{
	PlayResult result = {};

	if (plays.off == PLAY_OFF_KNEEL) {
		PlaySim_Kneel(sbData, &result);
	} else if (PlaySim_IsSpecialTeamsPlay(plays.off)) {
		PlaySim_SpecialTeamsPlay(sbData, plays.off, &result);
	} else {
		PlaySim_StandardPlay(sbData, plays, &result);
	}

	//Calculate net Yardage - note in MVP there is not "order of play" in the eventual play summary it will just be: Offense fumbled, recovered by the defense, net yards 15 or whatever so like offense ran it 20 yards, fumbled, recovered by defense, then tackled after 5 is the same as offense fumbled at LoS but the dumb linebacker ran backwards 20 yards before being tackled. Eventually I will have the nuance but...NOT MVP
	result.netYards = result.offYards - result.defYards;

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
	switch (play) {
		case PLAY_OFF_KICK:
			PlaySim_Kick(sbData, result);
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
	switch (plays.off) {
		case PLAY_OFF_RUN:
			PlaySim_Run(result, plays.def, sbData->los);
			break;
		case PLAY_OFF_SHORT_PASS:
			PlaySim_ShortPass(sbData, result);
			break;
		case PLAY_OFF_LONG_PASS:
			PlaySim_LongPass(sbData, result);
			break;
		default:
			//ERROR
			printf("playID OOB PlaySim_SpecialTeamsPlay()\n");
			PlaySim_Kneel(sbData, result);
			break;
	}

	PlaySim_ResolvePlay(sbData, result);
}

static void PlaySim_CheckFumble(PlayResult *result)
{

	s32 roll = rand() % 100;
	
	//It's a fumble! Check who recovers
	if (roll < 1) {
		
		roll = rand() % 2;
		if (roll < 1) {
			//Defense recovers
			result->isFumble   = true;
			result->isTurnover = true;
		
			//Check yards after recover
			roll = rand() % NUM_PLAY_OUTCOMES;
			s32 gain = FUMBLE_DEF_RECOVERY[roll];
			result->netYards -= gain;
			result->defYards += gain;
		}
	}
}

//Everything that happens when the "forward progress" stops. 
//This gets called after a run, or after a pass + run after pass (which includes an int)
//This func checks for the fumble and scores it also caps yards at field length so no 50 yard run from the 10
static void PlaySim_ResolvePlay(const ScoreboardData *sb, PlayResult *result)
{

	//1: Check if ball went down out of bounds -> score
	s32 fieldLen = PlaySim_GetFieldLength(sb->session.pos, sb->los);

	//TODO: This is where I left off - Check the FIXME as well
	//2: Check Fumble
	PlaySim_CheckFumble(result);

	//3: If Fumble check if ball went down out of bounds again -> score / touchback (not mvp!)

	
	//Check offense touchdown
	if (PlaySim_ResolvePlay_CheckTouchdown(fieldLen, result)) {
		return;
	}
	
	//Check Safety
	if (PlaySim_ResolvePlay_CheckSafety(fieldLen, result)) {
		return;
	}

	//Check Defense touchdown
	

}

static bool PlaySim_ResolvePlay_CheckTouchdown(const s32 fieldLen, PlayResult *result)
{

	//Compare netYards with the field length
	if (result->netYards > fieldLen) {
		result->pointsScored = gScoreTable[SCORE_TOUCHDOWN];
		result->netYards = fieldLen;
		return true;
	}

	return false;
}

static bool PlaySim_ResolvePlay_CheckSafety(const s32 fieldLen, PlayResult *result)
{
	//Only possible on a play that went backwards
	//Defensive safety is pretty rare, just gonna ignore it for now, technically that would be a different func i think and also it's normally an "impetus" which is just a touchback
	if (result->netYards >= 0 || result->isTurnover) {
		return false;
	}
	s32 backFieldLen = 100 - fieldLen;
	if (backFieldLen + result->netYards <= 0) {
		result->pointsScored = gScoreTable[SCORE_SAFETY];
		result->netYards = 0 - backFieldLen;
		return true;
	}

	return false;
}

//Run is a "safe" call
//Average 3-4 yards
//Run can get tackled in the backfield for a loss
//Run cannot be intercepted or dropped
//Run can fumble the ball (1%)
//NOTE: Run doesn't need the sbData (only los is even relevent anyways!) because you dont need space to drop back or space for the play to develop etc
static void PlaySim_Run(PlayResult *result, const PlayID def, const s32 los)
{
	//FIXME
	(void)los;
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
	result->offYards += gain;
	result->netYards += gain;
}


//NOTE: When you get here we don't need to pass the entire sbData just the los
static void PlaySim_ShortPass(const ScoreboardData *sbData, PlayResult *result)
{
	(void) result;
	(void) sbData;
}

static void PlaySim_LongPass(const ScoreboardData *sbData, PlayResult *result)
{
	(void) result;
	(void) sbData;
}

static void PlaySim_Kneel(const ScoreboardData *sbData, PlayResult *result)
{
	result->offYards = -1;

	//Check safety
	if (sbData->los <= 1) {
		result->pointsScored = 2;
		result->defScored = true;
	}
}

static void PlaySim_Kick(const ScoreboardData *sbData, PlayResult *result)
{
	(void) result;
	(void) sbData;
}

static void PlaySim_Punt(const ScoreboardData *sbData, PlayResult *result)
{
	(void) result;
	(void) sbData;
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




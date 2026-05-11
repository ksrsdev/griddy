#include "play_sim.h"

#include <stdio.h>

#include "play_calling.h"


static bool PlaySim_IsSpecialTeamsPlay(const PlayID play);
static void PlaySim_SpecialTeamsPlay(const ScoreboardData *sbData, const PlayID play, PlayResult *result);

static void PlaySim_StandardPlay(const ScoreboardData *sbData, const PlayMatchup plays, PlayResult *result);

//Play specific funcs
static void PlaySim_Run(const ScoreboardData *sbData, PlayResult *result);
static void PlaySim_ShortPass(const ScoreboardData *sbData, PlayResult *result);
static void PlaySim_LongPass(const ScoreboardData *sbData, PlayResult *result);
static void PlaySim_Kneel(const ScoreboardData *sbData, PlayResult *result);
static void PlaySim_Kick(const ScoreboardData *sbData, PlayResult *result);
static void PlaySim_Punt(const ScoreboardData *sbData, PlayResult *result);

static constexpr u32 NUM_OFF_PLAYS =  PLAY_OFF_END - PLAY_OFF_START;
static constexpr u32 NUM_DEF_PLAYS =  PLAY_DEF_END - PLAY_DEF_START;

typedef enum {
	PLAY_ADVANTAGE_NONE,
	PLAY_ADVANTAGE_OFF,
	PLAY_ADVANTAGE_DEF,
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
		PLAY_ADVANTAGE_OFF, //Run
		PLAY_ADVANTAGE_NONE, //Short Pas
		PLAY_ADVANTAGE_DEF  //Long Pass
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

	//Obviously needs a massive amount of expansion
	if (plays.off == PLAY_OFF_KICK && sbData->los < 20) {
		result.score = true;
		result.yardsGained = 0;
		result.turnover = true;
		result.fieldGoal = true;
		result.playerScore = true;
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

	PlayAdvantage adv = sPlayAdvantageTable[plays.def - PLAY_DEF_START][plays.off - PLAY_OFF_START];
	(void)adv;

//	printf("adv: %d\n", adv);

	switch (plays.off) {
		case PLAY_OFF_RUN:
			PlaySim_Run(sbData, result);
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
}

static void PlaySim_Run(const ScoreboardData *sbData, PlayResult *result)
{
	(void) sbData;
	(void) result;
}

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




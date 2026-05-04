#include "play_sim.h"

#include "play_calling.h"


PlayResult PlaySim_Main(const ScoreboardData *sbData, const PlayMatchup plays)
{
	PlayResult result = {};

	//Obviously needs a massive amount of expansion
	if (plays.off == PLAY_OFF_KICK && sbData->los < 20) {
		result.score = true;
		result.yardsGained = 0;
		result.turnover = true;
		result.fieldGoal = true;
		result.playerScore = true;
	}
	
	return result;
}

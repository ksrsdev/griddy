#include "global.h"
#include "player.h"
#include "roster.h"
#include "team.h"

#include <stdlib.h>

void UnloadRoster(Player **roster, long unsigned int *count) 
{
	if (roster == NULL || *roster == NULL) {
		return;
	}
	free (*roster);
	*roster = NULL;
	*count = 0;
}

int ComparePlayers_ReturnLargerOvr(const void *a, const void *b)
{
	return((Player *)b)->overall - ((Player *)a)->overall;
}

//Clearly needs some work but this is the gist of it
ErrorType LoadRoster(const TeamId teamId, Player **roster, long unsigned int *count)
{
	//First confirm all the arguments are valid
	if (*roster != NULL) {
		TraceLog(LOG_ERROR, "ERROR: roster pointer not NULL - LoadRoster()");
		return ERROR_GLOBAL_CTX;
	}
	if (teamId <= TEAM_NONE + 1 || teamId >= TEAM_COUNT)
	{
		TraceLog(LOG_ERROR, "ERROR: teamId OOB - LoadRoster()");
		return ERROR_GLOBAL_CTX;
	}
	if (*count != 0) {
		TraceLog(LOG_ERROR, "ERROR: count not 0 - LoadRoster()");
		return ERROR_GLOBAL_CTX;
	}
	const TeamData *teamData = GetTeamData(ctx.playerTeamId);
	//designate the file to load roster from
	char rosterFileName[32] = {0};
	snprintf(rosterFileName, sizeof(rosterFileName), "%s.roster", teamData->blueprint->name);
	//create FILE pointer
	FILE *rosterFile = fopen(rosterFileName, "rb");
	//confirm pointer is not NULL
	if (rosterFile == NULL) {
		TraceLog(LOG_ERROR, "ERROR: rosterFile does not exist!");
		return ERROR_FILE_R;
	}
	//get roster numLines
	char stringBuffer[256] = {0};
	int numLines = 0;
	while (fgets(stringBuffer, sizeof(stringBuffer), rosterFile)) {
		numLines++;
	}
	//save numLines into global ctx (to use in free function later on!)
	*count = (long unsigned int)numLines;
	//rewind file pointer
	rewind(rosterFile);
	//create a player roster[] array of size numLines
	//global playerRoster points there
	*roster = calloc((long unsigned int)numLines, sizeof(Player));
	//confirm allocation success
	if (*roster == NULL) {
		fclose (rosterFile);
		return ERROR_ALLOCATION;
	}
	for (int i=0; i<(int)numLines; i++) {
		if (fgets(stringBuffer, sizeof(stringBuffer), rosterFile)) {
			int tempPos;
			int dataLoaded = sscanf(stringBuffer, "%31[^|]|%31[^|]|%d|%d|%d", 
					(*roster)[i].firstName,
					(*roster)[i].lastName,
					&tempPos,
					&(*roster)[i].number,
					&(*roster)[i].overall);
			//Data loaded incorrectly, return ERROR
			if (dataLoaded != 5) {
				TraceLog(LOG_INFO, "Player on line %d Loaded Incorrectly", i);
				fclose(rosterFile);
				free(*roster);
				*roster = NULL;
				return ERROR_FILE_R;
			}
			//Data loaded correctly - handle tempPos
			(*roster)[i].position = (PlayerPosition)tempPos;
		}
		//Fgets read incorrectly, return ERROR
		else {
			TraceLog(LOG_INFO, "fgets returned NULL at line %d", i);
			free(*roster);
			*roster = NULL;
			fclose(rosterFile);
			return ERROR_FILE_R;
		}
	}
	//after it's all done close the file
	fclose (rosterFile);
	return ERROR_NONE;
}

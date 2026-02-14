#define _DEFAULT_SOURCE

#include "player.h"
#include "team.h"
#include "util.h"

#include "raylib.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

static const int POSITION_BASE_NUMBER[POSITION_COUNT] = {
	[POSITION_NONE] = 0,
	[POSITION_TACKLE] = 60,
	[POSITION_GUARD] = 60,
	[POSITION_CENTER] = 60,
	[POSITION_QUARTER_BACK] = 0,
	[POSITION_FULL_BACK] = 10,
	[POSITION_HALF_BACK] = 20,
	[POSITION_TIGHT_END] = 80,
	[POSITION_WIDE_RECEIVER] = 30,
	//Defense
	[POSITION_DEFENSIVE_TACKLE] = 90,
	[POSITION_DEFENSIVE_END] = 90,
	[POSITION_LINE_BACKER] = 50,
	[POSITION_DEFENSIVE_BACK] = 40,
//	[POSITION_CORNER_BACK] = 40,
//	[POSITION_SAFETY] = 40,
	//Special Teams
	[POSITION_KICKER] = 70
};

const char *positionNames[POSITION_COUNT] = {
	"NONE",
	"T",
	"G",
	"C",
	"TE",
	"RB",
	"QB",
	"DE",
	"DT",
	"LB",
	"DB",
	"K",
};

typedef enum {
	ERROR_NONE,
	ERROR_FILE,
	ERROR_JERSEY_NUMBER,
	ERROR_GEN_STATS,
	ERROR_COUNT
} RosterGenErrorCode;

int AssignPlayerName(Player *player, int firstOrLast);
int GeneratePlayerForRoster(const PlayerPosition pos, const PlayerStatMod statMod, bool *jerseyNumberTaken, FILE *rosterFile);
bool JerseyNumerInRangeForPosition(int num, PlayerPosition pos);

RosterGenErrorCode GenerateRosterForTeam(TeamId id);

#define ASSIGN_FIRST_NAME 1
#define ASSIGN_LAST_NAME  2

int AssignPlayerName(Player *player, int firstOrLast)
{
	FILE *namesFile;
	if (firstOrLast == ASSIGN_FIRST_NAME) {
		namesFile = fopen("data/first_names_formatted.txt", "r");
	} else if (firstOrLast == ASSIGN_LAST_NAME) {
		namesFile = fopen("data/last_names_formatted.txt", "r");
	} else {
		TraceLog(LOG_INFO, "ERROR: firstOrLast OOB\n");
		return 1;
	}
	if (namesFile == NULL) {
		TraceLog(LOG_INFO, "ERROR: Unable to open file!\n");
		return 1;
	}
	int numLines = 0;
	char stringBuffer[128];
	//First get the number of lines
	while (fgets(stringBuffer, 128, namesFile)) {
		numLines++;
	}
	//reset pointer to top of file
	rewind(namesFile); 
	//create the array to hold the names
	char **names = calloc((long unsigned int)numLines, (long unsigned int)numLines * sizeof(char *));
	//load names into array
	int i=0;
	while (i<numLines && fgets(stringBuffer, 128, namesFile)) {
		stringBuffer[strcspn(stringBuffer, "\n")] = 0;
		names[i] = strdup(stringBuffer);
		i++;
	}
	//Need to use strdup not pointers (player.firstName = firstNames[randomNumber])because the array is being freed after
	if (numLines > 0) {
		int randomNumber = rand() % numLines;
		if (firstOrLast == ASSIGN_FIRST_NAME) {
			strcpy(player->firstName, names[randomNumber]);
		} else {
			strcpy(player->lastName, names[randomNumber]);
		}
	} else {
		TraceLog(LOG_INFO, "Error: loaded namesFile was empty");
	}
	if (names != NULL) {
		//Free strings
		for (int j = 0; j < numLines; j++) {
			free(names[j]); 
		}
		//Free array itself
		free(names);
	}
	fclose(namesFile);

	return 0;
}

//This will only work on this test function where there is a single player in a file on a sinlge line like first|last|pos|num|ovr 
void TestLoadPlayerFromFile(void) {
	FILE *file = fopen("output.txt", "r");
	if (file == NULL) {
		TraceLog(LOG_INFO, "ERROR: Unable to open savedData file!\n");
		return;
	}
	char stringBuffer[128];
	if (fgets(stringBuffer, 128, file) == NULL) {
		TraceLog(LOG_INFO, "ERROR! fgets(savedData) failed!");
		return;
	}
	stringBuffer[strcspn(stringBuffer, "\n")] = 0;
	Player player;
	int tempPos;
	int dataLoaded = sscanf(stringBuffer, "%31[^|]|%31[^|]|%d|%d|%d", 
			player.firstName,
			player.lastName,
			&tempPos,
			&player.number,
			&player.overall);
	if (dataLoaded != 5) {
		TraceLog(LOG_INFO, "Player Loaded Incorrectly");
	} else {
		player.position = (PlayerPosition)tempPos;
		TraceLog(LOG_INFO, "First: %s\nLast: %s\nPos: %s\nNum: %d\nOvr: %d\n", player.firstName, player.lastName, positionNames[player.position], player.number, player.overall);
	}
	fclose(file);
}

//Just totally random / test function
int GenRandomPlayer(void)
{
	//Create player struct
	Player player;
	int randNum;
	//Populate it with random data
	//Names
AssignPlayerName(&player, ASSIGN_FIRST_NAME);
	AssignPlayerName(&player, ASSIGN_LAST_NAME);
	//Position
	randNum = (rand() % (POSITION_COUNT - POSITION_TACKLE)) + POSITION_TACKLE;
	player.position = randNum;
	//number
	randNum = rand() % 100;
	player.number = randNum;
//age
//	randNum = (rand() % (41 - 18 + 1)) + 18;
//	player.age = randNum; 
	//height
	//weight
	//speed
	//acceleration
	//strength
	//agility
	//jumping
	//awareness
	//decisions
	//composure
	//workrate
	//toughness
	//throwing
	//catching
	//route_running
	//blocking
	//tackling
	//coverage
	//kicking
	//overall
	randNum = rand() % 100;
	player.overall = randNum;
	//return created player
	TraceLog(LOG_INFO, "%s|%s|%s|%d|%d", player.firstName, player.lastName, positionNames[player.position], player.number, player.overall);
	FILE *outputFile = fopen("output.txt", "w");
	if (outputFile == NULL) {
		TraceLog(LOG_INFO, "output.txt failed to open");
		return 1;
	}
	fprintf(outputFile, "%s|%s|%d|%d|%d", player.firstName, player.lastName,player.position, player.number, player.overall);
	fclose(outputFile);
	//Test load info from file
	FILE *savedData = fopen("output.txt", "r");
	if (savedData == NULL) {
		TraceLog(LOG_INFO, "ERROR: Unable to open savedData file!\n");
		return 1;
	}
	char stringBuffer[128];
	if (fgets(stringBuffer, 128, savedData) != NULL) {
		TraceLog(LOG_INFO, "From Saved Data: %s", stringBuffer);
	} else {
		TraceLog(LOG_INFO, "ERROR! fgets(savedData) failed!");
	}
	fclose(savedData);
	
	return 0;
}

int GeneratePlayerForRoster(const PlayerPosition pos, const PlayerStatMod statMod, bool *jerseyNumberTaken, FILE *rosterFile)
{
	Player player;
	//name
	AssignPlayerName(&player, ASSIGN_FIRST_NAME);
	AssignPlayerName(&player, ASSIGN_LAST_NAME);
	//pos
	player.position = pos;
	//number
	//First check to make sure we don't enter an infinite loop
	bool numberExists = false;
	for (int i=0; i<100; i++) {
		if (JerseyNumerInRangeForPosition(i, pos) && jerseyNumberTaken[i] == false) {
			numberExists = true;
			break;
		}
	}
	if (numberExists) {
		int randNum = rand() % 100;
		while (jerseyNumberTaken[randNum] == true || !(JerseyNumerInRangeForPosition(randNum, pos))) {
			randNum = rand() % 100;
		}
		player.number = randNum;
		jerseyNumberTaken[randNum] = true;
	} else {
		TraceLog(LOG_INFO, "ERROR: Jersey Number Not Found!");
		return ERROR_JERSEY_NUMBER;
	}
	//stat
	int mean, stdDev;
	switch (statMod) {
		case PLAYER_STAT_MOD_NONE:
		case PLAYER_STAT_MOD_STANDARD:
			mean = 50;
			stdDev = 15;
			break;
		case PLAYER_STAT_MOD_BUFF:
			mean = 75;
			stdDev = 10;
			break;
		case PLAYER_STAT_MOD_DEBUFF:
			mean = 25;
			stdDev = 10;
			break;
		case PLAYER_STAT_MOD_STAR:
			mean = 95;
			stdDev = 3;
			break;
		case PLAYER_STAT_MOD_COUNT:
		default:
			TraceLog(LOG_ERROR, "ERROR: statMod oob: %d\n", statMod);
			return ERROR_GEN_STATS;
	}
	int randNum = GenerateNormal(mean, stdDev);
	if (randNum > 100) {
		randNum = 100;
	}
	if (randNum < 0) {
		randNum = 0;
	}
	player.overall = randNum;

	fprintf(rosterFile, "%s|%s|%d|%d|%d\n", player.firstName, player.lastName,player.position, player.number, player.overall);
	return ERROR_NONE;
}

#undef ASSIGN_FIRST_NAME
#undef ASSIGN_LAST_NAME

//If jersey number in range return true
//else return false
bool JerseyNumerInRangeForPosition(int num, PlayerPosition pos)
{
	if (num >= POSITION_BASE_NUMBER[pos] && num < POSITION_BASE_NUMBER[pos] + 10) {
		return true;
	} else {
		return false;
	}
}

//Generate all players for each team and save them to individual files (white.roster, black.roster etc)
int GenerateAllRosters(void)
{
	RosterGenErrorCode errorWatcher = ERROR_NONE;
	for (int i=(TEAM_RANDOM+1);i<TEAM_COUNT;i++) {
		errorWatcher = GenerateRosterForTeam(i);
		if (errorWatcher != ERROR_NONE) {
			const TeamData *teamData = GetTeamData(i);
			TraceLog(LOG_ERROR, "ERROR: GenerateRosterForTeam(%s)\nERROR CODE: %d\n", teamData->blueprint->name, errorWatcher);
			return 1;
		}
	}
	TraceLog(LOG_INFO, "All Rosters finished successfully!");
	return 0;
}

RosterGenErrorCode GenerateRosterForTeam(TeamId id) 
{
	//just placeholder to quiet warning
	const TeamData *teamData = GetTeamData(id);
	char rosterFileName[32];
	snprintf(rosterFileName, sizeof(rosterFileName), "%s.roster", teamData->blueprint->name);
	TraceLog(LOG_INFO, "%s\n", rosterFileName);
	FILE *rosterFile = fopen(rosterFileName, "wb");
	if (rosterFile == NULL) {
		return ERROR_FILE;
	}
	bool jerseyNumberTaken[100] = {true};
	RosterGenErrorCode errorFound = ERROR_NONE;
	//For each position
	//i is POSITION
	//j is number of players in that position for this roster
	for (int i = (POSITION_NONE + 1); i<POSITION_COUNT; i++) {
		const int numPosition = teamData->blueprint->rosterSchema[i].numPosition;
		const int baseMod = teamData->blueprint->rosterSchema[i].statMod;
		for (int j=0; j<numPosition; j++) {
			int activeMod;
			if (baseMod == PLAYER_STAT_MOD_STAR && j>0) {
				activeMod = PLAYER_STAT_MOD_BUFF;
			} else {
				activeMod = baseMod;
			}
			errorFound = GeneratePlayerForRoster(i, activeMod, jerseyNumberTaken, rosterFile);
			if (errorFound != ERROR_NONE) {
				break;
			}
		}
	}
	fclose(rosterFile);
	return errorFound;
}



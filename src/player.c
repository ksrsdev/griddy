#define _DEFAULT_SOURCE

#include "player.h"
#include "team.h"

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
	[POSITION_CORNER_BACK] = 40,
	[POSITION_SAFETY] = 40,
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
	"CB",
	"S",
	"K",
	"P",
	"LS",
};

typedef enum {
	ERROR_NONE,
	ERROR_FILE,
	ERROR_GEN,
	ERROR_COUNT
} RosterGenErrorCode;

int AssignPlayerName(Player *player, int firstOrLast);
int GeneratePlayerForRoster(const PlayerPosition pos, const PlayerStatMod statMod, bool *jerseyNumberTaken, FILE *rosterFile);
bool JerseyNumerInRangeForPosition(int num, PlayerPosition pos);

int GenerateBlackRoster(void);
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
		return 1;
	}
	//stat
	int statMin, statMax;
	statMin = 20;
	statMax = 40;
	if (statMod == PLAYER_STAT_MOD_BUFF) {
		statMin += 20;
		statMax += 20;
	}
	if (statMod == PLAYER_STAT_MOD_DEBUFF) {
		statMin -= 20;
		statMax -= 20;
	}
	if (statMod == PLAYER_STAT_MOD_STAR) {
		statMin = 90;
		statMax = 100;
	}
	int randNum = rand() % (statMax - statMin) + statMin;
	player.overall = randNum;

	fprintf(rosterFile, "%s|%s|%d|%d|%d\n", player.firstName, player.lastName,player.position, player.number, player.overall);
	return 0;
}

#undef ASSIGN_FIRST_NAME
#undef ASSIGN_LAST_NAME

//If jersey number in range return true
//else return false
bool JerseyNumerInRangeForPosition(int num, PlayerPosition pos)
{
	if (num >= POSITION_BASE_NUMBER[pos] && num < POSITION_BASE_NUMBER[pos] + 10) {
		return true;
	}
	return false;
}

//Generate all players for each team and save them to individual files (white.roster, black.roster etc)
int GenerateAllRosters(void)
{
	RosterGenErrorCode errorWatcher = ERROR_NONE;
	for (int i=(TEAM_RANDOM+1);i<TEAM_COUNT;i++) {
		errorWatcher = GenerateRosterForTeam(i);
		if (errorWatcher != ERROR_NONE) {
			TraceLog(LOG_ERROR, "ERROR: GenerateRosterForTeam(%d)\nGenerateAllRosters exit %d\n", i);
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
	snprintf(rosterFileName, sizeof(rosterFileName), "%s.roster", teamData->name);
	TraceLog(LOG_INFO, "%s\n", rosterFileName);
	FILE *rosterFile = fopen(rosterFileName, "wb");
	bool jerseyNumberTaken[100] = {true};
	//For each position
	for (int i = (POSITION_NONE + 1); i<POSITION_COUNT; i++) {
		//For number of players in position for this roster
		for (int j = 0; i<teamData->rosterSchema[i].numPosition; j++) {
			GeneratePlayerForRoster(i, teamData->rosterSchema[i].statMod, jerseyNumberTaken, rosterFile);
		}
	}
	fclose(rosterFile);
	return ERROR_NONE;
}

//	Black   Bunch 11 | 3-4 Blitz | RB,WR,CB,LB,DL,S | QB,OL,TE,K | DT (NT) 
int GenerateBlackRoster(void) {

	//Open file for writing
	FILE *rosterFile = fopen("black.roster", "wb");
	if (rosterFile == NULL) {
		TraceLog(LOG_INFO, "rosterFile failed to open in wb mode");
		return 1;
	}
	//Create bool array to hold jerseyNumberTaken
	bool jerseyNumberTaken[100] = {true}; //only the first index is set true...C thang
	//T-
	GeneratePlayerForRoster(POSITION_TACKLE, PLAYER_STAT_MOD_DEBUFF, jerseyNumberTaken, rosterFile);
	//G-
	GeneratePlayerForRoster(POSITION_GUARD, PLAYER_STAT_MOD_DEBUFF, jerseyNumberTaken, rosterFile);
	//C-
	GeneratePlayerForRoster(POSITION_CENTER, PLAYER_STAT_MOD_DEBUFF, jerseyNumberTaken, rosterFile);
	//G-
	GeneratePlayerForRoster(POSITION_GUARD, PLAYER_STAT_MOD_DEBUFF, jerseyNumberTaken, rosterFile);
	//T-
	GeneratePlayerForRoster(POSITION_TACKLE, PLAYER_STAT_MOD_DEBUFF, jerseyNumberTaken, rosterFile);
	//QB-
	//HB+
	//TE-
	//WR+
	//WR+
	//WR+

	//DT++
	//DE+
	//DE+
	//LB+
	//LB+
	//LB+
	//LB+
	//CB+
	//CB+
	//S+
	//S+
	
	//K-

	fclose(rosterFile);
	return 0;


}



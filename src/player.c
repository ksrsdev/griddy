#define _DEFAULT_SOURCE

#include "player.h"
#include "raylib.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

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

int AssignPlayerName(Player *player, int firstOrLast);

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

int GenerateRoster(void)
{
	//Default minimum roster:
	//T
	//G
	//C
	//G
	//T
	//QB
	//This stuff depends on personell being used
	//Esentially you need 5 'recievers', with 2 on the Line
	//RB
	//TE
	//WR
	//Slot (RB, TE, WR)
	//Slot (RB, TE, WR)

	return 0;
}

void TestLoadRosterFromFile(void)
{

}


#undef ASSIGN_FIRST_NAME
#undef ASSIGN_LAST_NAME

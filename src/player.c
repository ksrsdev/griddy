#include "player.h"
#include "raylib.h"

#define _DEFAULT_SOURCE
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


//const char *firstNames[] = {"Abe", "Bill", "Clint", "Dave"};
//const char *lastNames[] = {"Adams", "Black", "Clinton", "Davidson"};


//Just totally random / test function
Player GenRandomPlayer(void)
{
	Player player;

	FILE *firstNamesFile = fopen("data/first_names_formatted.txt", "r");
//	FILE *lastNamesFile = fopen("data/last_names_formatted.txt", "r");
	if (firstNamesFile == NULL) {
//	if (firstNamesFile == NULL || lastNamesFile == NULL) {
		TraceLog(LOG_INFO, "ERROR!\nUNABLE TO OPEN FILE!\n");
		return player;
	}

	int numLines = 0;
	char stringBuffer[128];
	//First Names
	
	//First get the number of lines
	while (fgets(stringBuffer, 128, firstNamesFile)) {
		numLines++;
	}
	TraceLog(LOG_INFO, "first namse %d lines", numLines);
	//reset pointer to top of file
	rewind(firstNamesFile); 
	//create the array to hold the names
	char **firstNames = malloc((long unsigned int)numLines * sizeof(char *));
	//load names into array
	int i=0;
	while (i<numLines && fgets(stringBuffer, 128, firstNamesFile)) {
		stringBuffer[strcspn(stringBuffer, "\n")] = 0;
		firstNames[i] = strdup(stringBuffer);
		i++;
	}

	//Last Names
//	char **lastNames = malloc(numlins * sizeof(char *));
//	TraceLog(LOG_INFO, "last names %d lines", numLines);
//	rewind(lastNamesFile);

//	randomNumber = ((long unsigned int)rand() % (long unsigned int)sizeof(firstNames) / (long unsigned int)sizeof(firstNames[0]));

	//Need to use strdup not pointers because the array is being freed after
	if (numLines > 0) {
		int randomNumber = rand() % numLines;
		player.firstName = strdup(firstNames[randomNumber]);
	} else {
		player.firstName = NULL;
	}
		
//	randomNumber = ((long unsigned int)rand() % (long unsigned int)sizeof(lastNames) / (long unsigned int)sizeof(lastNames[0]));
//	player.lastName = lastNames[randomNumber];

//	TraceLog(LOG_INFO, "%s - %s", player.firstName, player.lastName);
	TraceLog(LOG_INFO, "Random Name: %s", player.firstName);

	if (firstNames != NULL) {
		//Free strings
		for (int j = 0; j < numLines; j++) {
			free(firstNames[j]); 
		}
		//Free array itself
		free(firstNames);
	}

	fclose(firstNamesFile);
//	fclose(lastNamesFile);

	return player;

}

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
	if (firstNamesFile == NULL) {
		TraceLog(LOG_INFO, "ERROR!\nUNABLE TO OPEN FILE!\n");
		return player;
	}
	int numLines = 0;
	char stringBuffer[128];
	//First get the number of lines
	while (fgets(stringBuffer, 128, firstNamesFile)) {
		numLines++;
	}
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
	//Need to use strdup not pointers because the array is being freed after
	if (numLines > 0) {
		int randomNumber = rand() % numLines;
		player.firstName = strdup(firstNames[randomNumber]);
	} else {
		player.firstName = NULL;
	}
	if (firstNames != NULL) {
		//Free strings
		for (int j = 0; j < numLines; j++) {
			free(firstNames[j]); 
		}
		//Free array itself
		free(firstNames);
	}
	fclose(firstNamesFile);

	return player;

}

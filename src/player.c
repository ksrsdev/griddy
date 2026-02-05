#include "player.h"
#include "raylib.h"

#include <stdlib.h>
#include <time.h>


const char *firstNames[] = {"Abe", "Bill", "Clint", "Dave"};
const char *lastNames[] = {"Adams", "Black", "Clinton", "Davidson"};


//Just totally random / test function
Player GenRandomPlayer(void)
{
	Player player;

	srand((unsigned int)time(NULL));
	int randomNumber;
	randomNumber = ((long unsigned int)rand() % (long unsigned int)sizeof(firstNames) / (long unsigned int)sizeof(firstNames[0]));
	player.firstName = firstNames[randomNumber];
	randomNumber = ((long unsigned int)rand() % (long unsigned int)sizeof(lastNames) / (long unsigned int)sizeof(lastNames[0]));
	player.lastName = lastNames[randomNumber];

	TraceLog(LOG_INFO, "%s - %s", player.firstName, player.lastName);

	return player;


}

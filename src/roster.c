#include "player.h"
#include "roster.h"

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

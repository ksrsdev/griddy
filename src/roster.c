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


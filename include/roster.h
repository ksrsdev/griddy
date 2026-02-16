#ifndef GUARD_ROSTER_H
#define GUARD_ROSTER_H

#include "player.h"
#include "team.h"

typedef enum {
	ERROR_NONE,
	ERROR_GLOBAL_ROSTER_POINTER,
	ERROR_TEAM_ID,
	ERROR_ROSTER_FILE,
	ERROR_GLOBAL_ROSTER_COUNT,
	ERROR_ALLOCATION,
	ERROR_COUNT
}LoadRosterErrorCode;

void UnloadRoster(Player **roster, long unsigned int *count);
int ComparePlayers_ReturnLargerOvr(const void *a, const void *b);
LoadRosterErrorCode LoadRoster(const TeamId team, Player **roster, long unsigned int *count);

#endif

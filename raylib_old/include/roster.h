#ifndef GUARD_ROSTER_H
#define GUARD_ROSTER_H

#include "error.h"
#include "player.h"
#include "team.h"

void UnloadRoster(Player **roster, long unsigned int *count);
int ComparePlayers_ReturnLargerOvr(const void *a, const void *b);
ErrorType LoadRoster(const TeamId team, Player **roster, long unsigned int *count);

#endif

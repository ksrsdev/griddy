#ifndef GUARD_TEAM_H
#define GUARD_TEAM_H

#include "raylib.h"

typedef enum {
	TEAM_NONE,   
	TEAM_RANDOM,
	TEAM_BLACK, 
	TEAM_WHITE,  
	TEAM_GREEN,  
	TEAM_RED,  
	TEAM_PINK,   
	TEAM_BROWN,  
	TEAM_YELLOW, 
	TEAM_ORANGE,
	TEAM_BLUE,
	TEAM_COUNT
} Team;

typedef struct {
	Team id;
	const char *name;
	const char *desc;
	Color color;
} TeamData;

extern TeamData gTeamData[TEAM_COUNT];

void InitTeamData(void);

#endif

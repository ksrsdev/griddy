#ifndef GUARD_TEAM_H
#define GUARD_TEAM_H

#include "player.h"

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
} TeamId;

typedef struct {
	int numPosition;
	PlayerStatMod statMod;
} TeamPositionMod;

typedef struct {
	TeamId id;
	const char *name;
	TeamPositionMod rosterSchema[POSITION_COUNT];
} TeamBlueprint;

typedef struct {
	TeamId id;
	const char *name;
	Color color;
	TeamPositionMod rosterSchema[POSITION_COUNT];
} TeamData;

extern TeamData gTeamData[TEAM_COUNT];

void InitTeamData(void);

const TeamData* GetTeamData(TeamId id);

#endif

#ifndef GUARD_TEAM_H
#define GUARD_TEAM_H

#include <SDL3/SDL.h>

typedef enum {
	TEAM_ID_NONE,   
	TEAM_ID_RANDOM,
	TEAM_ID_BLACK, 
	TEAM_ID_WHITE,  
	TEAM_ID_GREEN,  
	TEAM_ID_RED,  
	TEAM_ID_PINK,   
	TEAM_ID_BROWN,  
	TEAM_ID_YELLOW, 
	TEAM_ID_ORANGE,
	TEAM_ID_BLUE,
	TEAM_ID_COUNT
} TeamID;

typedef struct {
	SDL_Color color;
	const char *title;
	const char *desc;
	const char *pros;
	const char *cons;
	const char *off;
	const char *def;
} TeamDescription;

typedef struct {
	TeamID player;
	TeamID cpu;
	TeamID focus;
} TeamAssignment;

extern TeamDescription gTeamDescriptions[TEAM_ID_COUNT];

void Team_ClearTeamAssignment(TeamAssignment *assignment);

#endif

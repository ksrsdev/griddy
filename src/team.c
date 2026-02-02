#include "team.h"
#include "raylib.h"


TeamData gTeamData[TEAM_COUNT];

void InitTeamData(void)  
{
	gTeamData[TEAM_NONE] = (TeamData){
		.id   = TEAM_NONE,
		.name = "NONE",
		.desc = "No team selected!",
		.color = BLACK,
	};
	
	gTeamData[TEAM_RANDOM] = (TeamData){
		.id   = TEAM_RANDOM,
		.name = "RANDOM ",
		.desc = "No team selected!",
		.color = BLACK,
	};
	
	gTeamData[TEAM_BLACK] = (TeamData){
		.id   = TEAM_BLACK,
		.name = "BLACK",
		.desc = "No team selected!",
		.color = BLACK,
	};
	
	gTeamData[TEAM_WHITE] = (TeamData){
		.id   = TEAM_WHITE,
		.name = "WHITE",
		.desc = "No team selected!",
		.color = BLACK,
	};
	
	gTeamData[TEAM_GREEN] = (TeamData){
		.id   = TEAM_GREEN,
		.name = "GREEN",
		.desc = "No team selected!",
		.color = BLACK,
	};
	
	gTeamData[TEAM_RED] = (TeamData){
		.id   = TEAM_RED,
		.name = "RED",
		.desc = "No team selected!",
		.color = BLACK,
	};
	
	gTeamData[TEAM_PINK] = (TeamData){
		.id   = TEAM_PINK,
		.name = "PINK",
		.desc = "No team selected!",
		.color = BLACK,
	};
	
	gTeamData[TEAM_BROWN] = (TeamData){
		.id   = TEAM_BROWN,
		.name = "BROWN",
		.desc = "No team selected!",
		.color = BLACK,
	};
	
	gTeamData[TEAM_YELLOW] = (TeamData){
		.id   = TEAM_YELLOW,
		.name = "YELLOW",
		.desc = "No team selected!",
		.color = BLACK,
	};
	
	gTeamData[TEAM_ORANGE] = (TeamData){
		.id   = TEAM_ORANGE,
		.name = "ORANGE",
		.desc = "No team selected!",
		.color = BLACK,
	};
	
	gTeamData[TEAM_BLUE] = (TeamData){
		.id   = TEAM_BLUE,
		.name = "BLUE",
		.desc = "No team selected!",
		.color = BLACK,
	};
}

const TeamData* GetTeamData(Team id)
{
	return &gTeamData[id];
}

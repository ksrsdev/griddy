#include "team.h"
#include "raylib.h"


TeamData gTeamData[TEAM_COUNT];

void InitTeamData(void)  
{
	gTeamData[TEAM_NONE] = (TeamData){
		.id   = TEAM_NONE,
		.name = "NONE",
		.pros = "No team selected!",
		.cons = "No team selected!",
		.color = BLACK,
	};
	
	gTeamData[TEAM_RANDOM] = (TeamData){
		.id   = TEAM_RANDOM,
		.name = "RANDOM ",
		.pros = "Random selected!",
		.cons = "Random selected!",
		.color = BLACK,
	};
	
	gTeamData[TEAM_BLACK] = (TeamData){
		.id   = TEAM_BLACK,
		.name = "BLACK",
		.pros = "RB, DB",
		.cons = "QB, OL, K, P",
		.color = BLACK,
	};
	
	gTeamData[TEAM_WHITE] = (TeamData){
		.id   = TEAM_WHITE,
		.name = "WHITE",
		.pros = "QB, OL, K, P ",
		.cons = "RB, DB",
		.color = WHITE,
	};
	
	gTeamData[TEAM_GREEN] = (TeamData){
		.id   = TEAM_GREEN,
		.name = "GREEN",
		.pros = "No team selected!",
		.cons = "No team selected!",
		.color = GREEN,
	};
	
	gTeamData[TEAM_RED] = (TeamData){
		.id   = TEAM_RED,
		.name = "RED",
		.pros = "No team selected!",
		.cons = "No team selected!",
		.color = RED,
	};
	
	gTeamData[TEAM_PINK] = (TeamData){
		.id   = TEAM_PINK,
		.name = "PINK",
		.pros = "No team selected!",
		.cons = "No team selected!",
		.color = PINK,
	};
	
	gTeamData[TEAM_BROWN] = (TeamData){
		.id   = TEAM_BROWN,
		.name = "BROWN",
		.pros = "No team selected!",
		.cons = "No team selected!",
		.color = BROWN,
	};
	
	gTeamData[TEAM_YELLOW] = (TeamData){
		.id   = TEAM_YELLOW,
		.name = "YELLOW",
		.pros = "No team selected!",
		.cons = "No team selected!",
		.color = YELLOW,
	};
	
	gTeamData[TEAM_ORANGE] = (TeamData){
		.id   = TEAM_ORANGE,
		.name = "ORANGE",
		.pros = "No team selected!",
		.cons = "No team selected!",
		.color = ORANGE,
	};
	
	gTeamData[TEAM_BLUE] = (TeamData){
		.id   = TEAM_BLUE,
		.name = "BLUE",
		.pros = "No team selected!",
		.cons = "No team selected!",
		.color = BLUE,
	};
}

const TeamData* GetTeamData(Team id)
{
	return &gTeamData[id];
}

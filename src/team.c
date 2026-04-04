#include "team.h"

#include "colors.h"

TeamDescription gTeamDescriptions[TEAM_ID_COUNT] = {
	[TEAM_ID_NONE] = {
		.color = COLOR_NONE,
		.title = "NONE",
		.desc  = "NONE",
		.pros  = "NONE",
		.cons  = "NONE",
		.off   = "NONE",
		.def   = "NONE"
	},
	[TEAM_ID_RANDOM] = {
		.color = COLOR_NONE,
		.title = "NONE",
		.desc  = "NONE",
		.pros  = "NONE",
		.cons  = "NONE",
		.off   = "NONE",
		.def   = "NONE"
	},
	[TEAM_ID_BLACK] = {
		.color = COLOR_BLACK,
		.title = "BLACK",
		.desc = "LESS THINKING, MORE HITTING.",
		.pros = "RB, WR, DL, LB, DB",
		.cons = "OL, QB, TE, K",
		.off  = "11 PISTOL",
		.def  = "3-4",
	},
	[TEAM_ID_WHITE] = {
		.color = COLOR_WHITE,
		.title = "WHITE",
		.desc  = "A GOOD PLAN BEATS BRUTE FORCE ANYDAY.",
		.pros  = "OL, QB, TE, K",
		.cons  = "RB, WR, DL, LB, DB",
		.off   = "11 GUN BUNCH Y FLEX",
		.def   = "4-3",
	},
	[TEAM_ID_GREEN] = {
		.color = COLOR_GREEN,
		.title = "GREEN",
		.desc  = "SCORE 50 POINTS OR GET SACKED TRYING.",
		.pros  = "QB, WR, LB",
		.cons  = "OL, DB, K",
		.off   = "10 SHOTGUN SPREAD",
		.def   = "3-4",
	},
	[TEAM_ID_RED] = {
		.color = COLOR_RED,
		.title = "RED",
		.desc  = "FIRST WE SCORE, THEN WE SCORE SOME MORE.",
		.pros  = "ALL OFFENSIVE POSITIONS",
		.cons  = "ALL DEFENSIVE POSITIONS",
		.off   = "21 PRO SET",
		.def   = "4-2",
	},
	[TEAM_ID_PINK] = {
		.color = COLOR_PINK,
		.title = "PINK",
		.desc  = "WHO NEEDS TO HIT HARD WHEN YOU CAN OUTRUN THE COMPETION?",
		.pros  = "WR, DB",
		.cons  = "OL, DL",
		.off   = "00 GUN EMPTY BUNCH TIGHT",
		.def   = "3-2-6",
	},
	[TEAM_ID_BROWN] = {
		.color = COLOR_BROWN,
		.title = "BROWN",
		.desc  = "THIS AIN'T YOUR GRANPA'S FOOTBALL TEAM, IT'S YOUR GREAT-GRANDPA'S.",
		.pros  = "OL, DL",
		.cons  = "QB, DB",
		.off   = "32 T FORMATION",
		.def   = "6-2-3",
	},
	[TEAM_ID_YELLOW] = {
		.color = COLOR_YELLOW,
		.title = "YELLOW",
		.desc  = "BIG BODIES BEAT SMALL BODIES EVERYTIME.",
		.pros  = "OL, DL, TE",
		.cons  = "WR, DB",
		.off   = "12 ACE H-BACK",
		.def   = "5-2-4",
	},
	[TEAM_ID_ORANGE] = {
		.color = COLOR_ORANGE,
		.title = "ORANGE",
		.desc  = "GOOD OLD WISHBONE, NOTHING BEATS THAT!",
		.pros  = "QB,FB,HB",
		.cons  = "WR,DL,K",
		.off   = "31 WISHBONE",
		.def   = "4-4",
	},
	[TEAM_ID_BLUE] = {
		.color = COLOR_BLUE,
		.title = "BLUE",
		.desc  = "YOU CAN'T LOSE IF YOUR OPPONENT CAN'T SCORE.",
		.pros  = "ALL DEFENSIVE POSITIONS.",
		.cons  = "ALL OFFENSIVE POSITIONS.",
		.off   = "21 I PRO",
		.def   = "4-3",
	},
};

void Team_ClearTeamAssignment(TeamAssignment *assignment)
{
	assignment->player = TEAM_ID_NONE;
	assignment->cpu = TEAM_ID_NONE;
	assignment->focus = TEAM_ID_NONE;
}

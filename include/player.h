#ifndef GUARD_PLAYER_H
#define GUARD_PLAYER_H

#include <stdio.h>

typedef enum {
	POSITION_NONE,
	//Offense
	POSITION_TACKLE,
	POSITION_GUARD,
	POSITION_CENTER,
	POSITION_QUARTER_BACK,
	POSITION_FULL_BACK,
	POSITION_HALF_BACK,
	POSITION_TIGHT_END,
	POSITION_WIDE_RECEIVER,
	//Defense
	POSITION_DEFENSIVE_TACKLE,
	POSITION_DEFENSIVE_END,
	POSITION_LINE_BACKER,
	POSITION_CORNER_BACK,
	POSITION_SAFETY,
	//Special Teams
	POSITION_KICKER,
//	POSITION_PUNTER,
//	POSITION_LONG_SNAPPER,
	POSITION_COUNT
} PlayerPosition;

//typedef enum {
//	PERSONALITY_NONE,
//	PERSONALITY_IMPUSLIVE,
//	PERSONALITY_CALCULATING,
//	PERSONALITY_COUNT
//} PlayerPersonality;

typedef struct {
	//basics
	char firstName[32];
	char lastName[32];
	PlayerPosition position;
	int number;
	//physical attributes
	int age;
	int height;
	int weight;
	int speed;
	int acceleration;
	int strength;
	int agility;
	int jumping;
	//mental
	int awareness;
	int decisions;
	int composure;
	int workrate;
	int toughness;
	//technical
	int throwing;
	int catching;
	int route_running;
	int blocking;
	int tackling;
	int coverage;
	int kicking;
	//misc
//	PlayerPersonality personality;
	int overall;
} Player;

int GenRandomPlayer(void);
void TestLoadPlayerFromFile(void);
int GenerateRoster(void);
int GenerateAllRosters(void);

#endif

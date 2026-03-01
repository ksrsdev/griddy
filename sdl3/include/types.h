#ifndef GUARD_TYPES_H
#define GUARD_TYPES_H

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint8_t  u8;

typedef enum {
	DIR_NONE,
	DIR_NORTH,
	DIR_SOUTH,
	DIR_EAST,
	DIR_WEST,
	DIR_COUNT
} Direction;

typedef struct {
	int x;
	int y;
} Vector2;

typedef struct {
	float x;
	float y;
} FVector2;


#endif

#ifndef GUARD_TYPES_H
#define GUARD_TYPES_H

#include <stdint.h>

//floating point
typedef double    f64;
typedef float     f32;

//unsigned ints
typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;

//signed ints
typedef int64_t s64;
typedef int32_t s32;
typedef int16_t s16;
typedef int8_t  s8;

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

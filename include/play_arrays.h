#ifndef GUARD_PLAY_ARRAYS_H
#define GUARD_PLAY_ARRAYS_H

constexpr u8 NUM_PLAY_OUTCOMES = 10;

//RUN
constexpr s32 RUN_ADVANTAGE_NONE[NUM_PLAY_OUTCOMES]    = {-1,  1,  2, 3, 3, 4, 4,   5,  6, 10};
constexpr s32 RUN_ADVANTAGE_OFF[NUM_PLAY_OUTCOMES]     = { 3,  4,  4, 5, 6, 8, 12, 18, 35, 80};
constexpr s32 RUN_ADVANTAGE_DEF[NUM_PLAY_OUTCOMES]     = {-3, -1,  0, 0, 1, 1, 2,   2,  3,  4};
constexpr s32 RUN_ADVANTAGE_PREVENT[NUM_PLAY_OUTCOMES] = { 4,  4,  5, 5, 6, 6, 7,   8,  9, 12};
constexpr s32 RUN_ADVANTAGE_CHAOS[NUM_PLAY_OUTCOMES]   = {-5, -3, -1, 0, 2, 4, 15, 25, 45, 80};


//SHORT PASS

//LONG PASS

//FUMBLE RECOVERY
constexpr s32 FUMBLE_DEF_RECOVERY[NUM_PLAY_OUTCOMES] = {0, 0, 0, 0, 0, 2, 5, 12, 30, 80};

//Sack loss
constexpr s32 SACK_TABLE_SHORT_PASS[NUM_PLAY_OUTCOMES]    = {-2, -3, -4, -4, -5, -5, -6, -7, -8, -10};
constexpr s32 SACK_TABLE_LONG_PASS[NUM_PLAY_OUTCOMES]     = {-4, -6, -7, -8, -9, -10, -11, -12, -14, -18};

#endif

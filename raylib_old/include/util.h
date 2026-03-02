#ifndef GUARD_UTIL_H
#define GUARD_UTIL_H

#include "raylib.h"

#include <stdbool.h>

float CycleFloatValue(float value, float max);
float CycleHue(float hue);
bool ColorsEqual(Color col1, Color col2);
int GenerateNormal(int mean, int stdDev);

#endif

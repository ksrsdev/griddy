#include "util.h"
#include "raylib.h"


float CycleFloatValue(float value, float max) 
{
	if (value >= max) {
		value = 0.0f;
	} else {
		value++;
	}
	return value;
}

float CycleHue(float hue)
{
	hue = CycleFloatValue(hue, 360.0f);
	TraceLog(LOG_INFO, "Hue: %f", (double)hue);
	return hue;
}

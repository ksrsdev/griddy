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
//	TraceLog(LOG_INFO, "Hue: %f", (double)hue);
	return hue;
}

bool ColorsEqual(Color col1, Color col2) {
    return (col1.r == col2.r && 
            col1.g == col2.g && 
            col1.b == col2.b && 
            col1.a == col2.a);
}

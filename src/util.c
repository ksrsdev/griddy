#include "util.h"
#include "raylib.h"

#include <stdlib.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832
#endif

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
/**
 * ELITE GENERATE NORMAL (Marsaglia Polar Method)
 * Faster and more robust than standard Box-Muller.
 */
int GenerateNormal(int mean, int stdDev) {
    static double spare;
    static int has_spare = 0;
    if (has_spare) {
        has_spare = 0;
        return (int)(spare * stdDev + mean);
    }
    double u, v, s;
    do {
        u = (double)rand() / (RAND_MAX / 2.0) - 1.0;
        v = (double)rand() / (RAND_MAX / 2.0) - 1.0;
        s = u * u + v * v;
    } while (s >= 1.0 || s == 0.0); 
	double mul = sqrt(-2.0 * log(s) / s);
    spare = v * mul;
    has_spare = 1;
    return (int)(u * mul * stdDev + mean);
}


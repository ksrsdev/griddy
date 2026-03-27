#include "splash_text.h"

#include <stdlib.h>

static const char *const sRandomStrings[] = {
#define X(str) str,
    SPLASH_TEXT(X)
#undef X
};

static const size_t sStringsCount = sizeof(sRandomStrings) / sizeof(sRandomStrings[0]);

const char* GetSplashText(void) {
    if (sStringsCount == 0) {
		return NULL;
	}
    
    int index = (size_t)rand() % sStringsCount;
    return sRandomStrings[index];
}

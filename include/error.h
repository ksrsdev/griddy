#ifndef GUARD_ERROR_H
#define GUARD_ERROR_H

#include <stdbool.h>

typedef enum {
	ERROR_NONE,
	ERROR_GLOBAL_CTX,
	ERROR_FILE_W,
	ERROR_FILE_R,
	ERROR_PLAYER_GEN,
	ERROR_ALLOCATION,
	ERROR_COUNT
} ErrorType;

typedef struct {
    const char *template;
    bool isErrorFatal;
} ErrorDefinition;


void TriggerError(ErrorType type, const char *message);
void DrawErrorScreen(void);

#endif

#ifndef GUARD_TEXT_H
#define GUARD_TEXT_H

void DrawMenuTitleText(const char *titleText);
void DrawTextInBox(const char *text, const Rectangle *textBoxRec);
void DrawTextInBoxColor(const char *text, const Rectangle *textBoxRec, const Color *color);

#define SPLASH_TEXT_COUNT 5

#endif

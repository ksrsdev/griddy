#ifndef GUARD_TEXT_H
#define GUARD_TEXT_H

void DrawInfoBoxTitleText(const char *titleText, const Rectangle *infoBox);
void DrawMenuTitleText(const char *titleText);
void DrawTextInBoxColor(const char *text, const Rectangle *textBox, const Color *color);

#define SPLASH_TEXT_COUNT 5

#endif

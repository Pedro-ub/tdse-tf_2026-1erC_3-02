#ifndef LCD_H_
#define LCD_H_

#include <stdint.h>

void lcdInit(void);
void lcdClear(void);
void lcdSetCursor(uint8_t col, uint8_t row);
void lcdPrint(const char *str);

void lcdUpdate(void);
void lcdShowTemporary(const char *line0, const char *line1);

#endif /* LCD_H_ */

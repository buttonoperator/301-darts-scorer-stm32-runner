/* lcd1602.h : HD44780 16x2 LCD, 4-bit mode, write-only (RW tied to ground). */
#ifndef LCD1602_H
#define LCD1602_H

#include "board.h"

void LCD_Init(void);
void LCD_Clear(void);
void LCD_SetCursor(uint8_t row, uint8_t col);   /* row 0 or 1, col 0 to 15 */
void LCD_Print(const char *s);
void LCD_PutChar(char c);

#endif /* LCD1602_H */

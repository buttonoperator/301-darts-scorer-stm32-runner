/* lcd_i2c.h : 16x2 HD44780 LCD behind a PCF8574 I2C backpack, 4-bit mode, write-only.
 * Every call takes the 7-bit backpack address, LCD_A_ADDR or LCD_B_ADDR. */
#ifndef LCD_I2C_H
#define LCD_I2C_H

#include "board.h"

HAL_StatusTypeDef LCDI2C_Init(uint8_t addr);
HAL_StatusTypeDef LCDI2C_Clear(uint8_t addr);
HAL_StatusTypeDef LCDI2C_SetCursor(uint8_t addr, uint8_t row, uint8_t col); /* row 0 or 1, col 0 to 15 */
HAL_StatusTypeDef LCDI2C_PutChar(uint8_t addr, char c);
HAL_StatusTypeDef LCDI2C_Print(uint8_t addr, const char *s);
HAL_StatusTypeDef LCDI2C_Backlight(uint8_t addr, uint8_t on);

#endif /* LCD_I2C_H */

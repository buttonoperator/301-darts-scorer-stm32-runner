/* lcd1602.c : HD44780 driver. Six lines pass through the 74HCT541, so every
 * line is an output and the busy flag is never read. Fixed delays are used. */
#include "lcd1602.h"

static void write_nibble(uint8_t n)
{
    HAL_GPIO_WritePin(LCD_PORT, LCD_D4_PIN, (n & 0x1U) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_PORT, LCD_D5_PIN, (n & 0x2U) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_PORT, LCD_D6_PIN, (n & 0x4U) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_PORT, LCD_D7_PIN, (n & 0x8U) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    Board_DelayUs(2);
    HAL_GPIO_WritePin(LCD_PORT, LCD_E_PIN, GPIO_PIN_SET);
    Board_DelayUs(2);                       /* E high at least 450 ns */
    HAL_GPIO_WritePin(LCD_PORT, LCD_E_PIN, GPIO_PIN_RESET);
    Board_DelayUs(2);
}

static void write_byte(uint8_t value, uint8_t is_data)
{
    HAL_GPIO_WritePin(LCD_PORT, LCD_RS_PIN, is_data ? GPIO_PIN_SET : GPIO_PIN_RESET);
    write_nibble((uint8_t)(value >> 4));
    write_nibble((uint8_t)(value & 0x0FU));
    Board_DelayUs(50);                      /* most commands take 37 us */
}

void LCD_Init(void)
{
    HAL_GPIO_WritePin(LCD_PORT, LCD_RS_PIN | LCD_E_PIN, GPIO_PIN_RESET);
    HAL_Delay(50);                          /* controller power-on time */

    /* Force 8-bit mode three times, then switch to 4-bit. */
    write_nibble(0x3); HAL_Delay(5);
    write_nibble(0x3); HAL_Delay(1);
    write_nibble(0x3); HAL_Delay(1);
    write_nibble(0x2); HAL_Delay(1);

    write_byte(0x28, 0);                    /* 4-bit, 2 lines, 5x8 font */
    write_byte(0x08, 0);                    /* display off */
    LCD_Clear();
    write_byte(0x06, 0);                    /* entry mode: increment, no shift */
    write_byte(0x0C, 0);                    /* display on, cursor off */
}

void LCD_Clear(void)
{
    write_byte(0x01, 0);
    HAL_Delay(2);                           /* clear takes 1.52 ms */
}

void LCD_SetCursor(uint8_t row, uint8_t col)
{
    write_byte((uint8_t)(0x80U | ((row ? 0x40U : 0x00U) + (col & 0x0FU))), 0);
}

void LCD_PutChar(char c)
{
    write_byte((uint8_t)c, 1);
}

void LCD_Print(const char *s)
{
    while (*s != '\0') {
        LCD_PutChar(*s++);
    }
}

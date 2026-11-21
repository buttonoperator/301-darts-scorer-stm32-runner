/* I2C LCDs: print on both rows of each LCD that answers. Needs lcd_i2c.c and board.c.
 * Set LCD_A_ADDR and LCD_B_ADDR in board.h to the addresses the scan found.
 * With only LCD A fitted, the LCD B calls return HAL_ERROR and lcd_b_ok stays 0. */

/* USER CODE BEGIN Includes */
#include "lcd_i2c.h"
/* USER CODE END Includes */

/* USER CODE BEGIN PV */
volatile uint8_t lcd_a_ok = 0, lcd_b_ok = 0;   /* watch in the debugger */
/* USER CODE END PV */

  /* USER CODE BEGIN 2 */
  if (LCDI2C_Init(LCD_A_ADDR) == HAL_OK) {
    lcd_a_ok = 1;
    LCDI2C_SetCursor(LCD_A_ADDR, 0, 0);
    LCDI2C_Print(LCD_A_ADDR, "PLAYER SIDE A");
    LCDI2C_SetCursor(LCD_A_ADDR, 1, 0);
    LCDI2C_Print(LCD_A_ADDR, "LCD A OK");
  }
  if (LCDI2C_Init(LCD_B_ADDR) == HAL_OK) {
    lcd_b_ok = 1;
    LCDI2C_SetCursor(LCD_B_ADDR, 0, 0);
    LCDI2C_Print(LCD_B_ADDR, "PLAYER SIDE B");
    LCDI2C_SetCursor(LCD_B_ADDR, 1, 0);
    LCDI2C_Print(LCD_B_ADDR, "LCD B OK");
  }
  /* USER CODE END 2 */

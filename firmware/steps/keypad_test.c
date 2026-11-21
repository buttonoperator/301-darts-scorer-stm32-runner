/* Keypad: show each decoded key on LCD A. Needs keypad.c, lcd_i2c.c and board.c. */

/* USER CODE BEGIN Includes */
#include "keypad.h"
#include "lcd_i2c.h"
/* USER CODE END Includes */

  /* USER CODE BEGIN 2 */
  LCDI2C_Init(LCD_A_ADDR);
  LCDI2C_Print(LCD_A_ADDR, "KEY:");
  /* USER CODE END 2 */

  /* USER CODE BEGIN WHILE */
  while (1)
  {
    char k = Keypad_GetKey();
    if (k != 0) {
      LCDI2C_SetCursor(LCD_A_ADDR, 0, 5);
      LCDI2C_PutChar(LCD_A_ADDR, k);
    }
    HAL_Delay(5);
    /* USER CODE END WHILE */

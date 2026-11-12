/* Keypad: show each decoded key on the LCD. Needs keypad.c and lcd1602.c. */

/* USER CODE BEGIN Includes */
#include "keypad.h"
#include "lcd1602.h"
/* USER CODE END Includes */

  /* USER CODE BEGIN 2 */
  LCD_Init();
  LCD_Print("KEY:");
  /* USER CODE END 2 */

  /* USER CODE BEGIN WHILE */
  while (1)
  {
    char k = Keypad_GetKey();
    if (k != 0) {
      LCD_SetCursor(0, 5);
      LCD_PutChar(k);
    }
    HAL_Delay(5);
    /* USER CODE END WHILE */

/* LCD: initialize and print on both rows. Needs lcd1602.c and board.c. */

/* USER CODE BEGIN Includes */
#include "lcd1602.h"
/* USER CODE END Includes */

  /* USER CODE BEGIN 2 */
  LCD_Init();
  LCD_SetCursor(0, 0);
  LCD_Print("301 DARTS SCORER");
  LCD_SetCursor(1, 0);
  LCD_Print("LCD OK  ROW 2");
  /* USER CODE END 2 */

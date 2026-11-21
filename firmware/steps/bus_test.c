/* Full I2C bus: both seven-segment displays and both LCDs together.
 * Needs ht16k33.c, lcd_i2c.c and board.c. */

/* USER CODE BEGIN Includes */
#include "ht16k33.h"
#include "lcd_i2c.h"
/* USER CODE END Includes */

  /* USER CODE BEGIN 2 */
  HT16K33_Init(DISP1_ADDR);
  HT16K33_Init(DISP2_ADDR);
  LCDI2C_Init(LCD_A_ADDR);
  LCDI2C_Init(LCD_B_ADDR);

  HT16K33_AllSegments(DISP1_ADDR, 1);
  HT16K33_AllSegments(DISP2_ADDR, 1);
  LCDI2C_Print(LCD_A_ADDR, "SEGMENT TEST");
  LCDI2C_Print(LCD_B_ADDR, "SEGMENT TEST");
  HAL_Delay(3000);                      /* inspect every segment */

  HT16K33_ShowNumber(DISP1_ADDR, 301);
  HT16K33_ShowNumber(DISP2_ADDR, 301);
  LCDI2C_Clear(LCD_A_ADDR);
  LCDI2C_Clear(LCD_B_ADDR);
  LCDI2C_Print(LCD_A_ADDR, "PLAYER 1   301");
  LCDI2C_Print(LCD_B_ADDR, "PLAYER 2   301");
  /* USER CODE END 2 */

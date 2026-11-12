/* I2C displays: full-segment test, then 301 on both. Needs ht16k33.c. */

/* USER CODE BEGIN Includes */
#include "ht16k33.h"
/* USER CODE END Includes */

  /* USER CODE BEGIN 2 */
  HT16K33_Init(DISP1_ADDR);
  HT16K33_Init(DISP2_ADDR);
  HT16K33_AllSegments(DISP1_ADDR, 1);
  HT16K33_AllSegments(DISP2_ADDR, 1);
  HAL_Delay(3000);                      /* inspect every segment */
  HT16K33_ShowNumber(DISP1_ADDR, 301);
  HT16K33_ShowNumber(DISP2_ADDR, 301);
  /* USER CODE END 2 */

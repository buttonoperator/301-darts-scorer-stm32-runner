/* Status: each LED channel alone, then three buzzer tones. Needs status.c. */

/* USER CODE BEGIN Includes */
#include "status.h"
/* USER CODE END Includes */

  /* USER CODE BEGIN WHILE */
  while (1)
  {
    RGB_Set(1, 0, 0); Buzzer_Beep(880, 150);  HAL_Delay(850);   /* score    */
    RGB_Set(0, 1, 0); Buzzer_Beep(220, 400);  HAL_Delay(600);   /* bust     */
    RGB_Set(0, 0, 1); Buzzer_Beep(1760, 300); HAL_Delay(700);   /* checkout */
    RGB_Set(0, 0, 0); HAL_Delay(1000);
    /* USER CODE END WHILE */

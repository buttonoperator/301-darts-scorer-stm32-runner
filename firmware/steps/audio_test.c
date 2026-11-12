/* Audio: 440 Hz tone, one second on and one second off. Needs audio.c.
 * Start with a low volume and raise it while measuring the 5 V rail. */

/* USER CODE BEGIN Includes */
#include "audio.h"
/* USER CODE END Includes */

  /* USER CODE BEGIN 2 */
  Audio_Init();
  /* USER CODE END 2 */

  /* USER CODE BEGIN WHILE */
  while (1)
  {
    Audio_Tone(440, 16);                /* raise toward AUDIO_VOL_MAX in steps */
    HAL_Delay(1000);
    Audio_Stop();
    HAL_Delay(1000);
    /* USER CODE END WHILE */

/* USER CODE BEGIN 4 */
/* If CubeMX already generated this callback (timer used as the HAL time base),
 * add only the TIM4 line to the existing function. */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM4) {
    Audio_TimerISR();
  }
}
/* USER CODE END 4 */

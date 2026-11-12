/* Buttons and pot: the pot sets display brightness, the buttons change the
 * number shown. Needs inputs.c and ht16k33.c. */

/* USER CODE BEGIN Includes */
#include "inputs.h"
#include "ht16k33.h"
/* USER CODE END Includes */

/* USER CODE BEGIN PV */
volatile uint16_t pot_raw;              /* watch in the debugger: 0 to 4095 */
/* USER CODE END PV */

  /* USER CODE BEGIN 2 */
  HT16K33_Init(DISP1_ADDR);
  /* USER CODE END 2 */

  /* USER CODE BEGIN WHILE */
  while (1)
  {
    pot_raw = ADC_ReadChannel(ADC_CH_POT);
    HT16K33_SetBrightness(DISP1_ADDR, Pot_ReadBrightness());
    if (Button_UndoDown()) {
      HT16K33_ShowNumber(DISP1_ADDR, 1);
    } else if (Button_NewGameDown()) {
      HT16K33_ShowNumber(DISP1_ADDR, 2);
    } else {
      HT16K33_ShowNumber(DISP1_ADDR, 0);
    }
    HAL_Delay(50);
    /* USER CODE END WHILE */

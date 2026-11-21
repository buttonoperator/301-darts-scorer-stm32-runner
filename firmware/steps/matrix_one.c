/* Matrix, one module: bring-up and current measurement. Needs max7219.c and board.c.
 * Only module 0 of chain A is fitted for this step.
 * Phase 1: all LEDs on at the intensity cap (0x05).
 * Phase 2: display-test mode, all LEDs at 31/32 duty. One module only. */

/* USER CODE BEGIN Includes */
#include "max7219.h"
/* USER CODE END Includes */

/* USER CODE BEGIN PV */
volatile uint8_t mx_phase = 0;          /* watch in the debugger: 1 = capped, 2 = display test */
/* USER CODE END PV */

  /* USER CODE BEGIN 2 */
  MX_Init(MX_CHAIN_A);
  MX_Fill(MX_CHAIN_A, 1);
  MX_Flush(MX_CHAIN_A);
  mx_phase = 1;
  HAL_Delay(10000);                     /* record the current at intensity 0x05 */

  MX_DisplayTest(MX_CHAIN_A, 1);
  mx_phase = 2;
  HAL_Delay(10000);                     /* record the current in display-test mode */

  MX_DisplayTest(MX_CHAIN_A, 0);
  MX_Fill(MX_CHAIN_A, 0);
  MX_Flush(MX_CHAIN_A);
  mx_phase = 0;
  /* USER CODE END 2 */

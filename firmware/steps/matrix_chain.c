/* Matrix chains: module order, then all LEDs on at the cap. Needs max7219.c and board.c.
 * Module m lights its top m+1 rows, so module 0 shows one row and module 7 shows all eight.
 * Never calls MX_DisplayTest: on a full chain that mode draws up to 2.6 A per chain. */

/* USER CODE BEGIN Includes */
#include "max7219.h"
/* USER CODE END Includes */

/* USER CODE BEGIN PV */
volatile uint8_t mx_full_on = 0;        /* watch in the debugger */
/* USER CODE END PV */

  /* USER CODE BEGIN 2 */
  for (uint8_t chain = MX_CHAIN_A; chain <= MX_CHAIN_B; chain++) {
    MX_Init(chain);
    for (uint8_t m = 0; m < MX_MODULES; m++) {
      for (uint8_t r = 0; r <= m; r++) {
        MX_SetRow(chain, m, r, 0xFF);
      }
    }
    MX_Flush(chain);
  }
  HAL_Delay(5000);                      /* check the order on both chains */

  for (uint8_t chain = MX_CHAIN_A; chain <= MX_CHAIN_B; chain++) {
    MX_Fill(chain, 1);
    MX_Flush(chain);
  }
  mx_full_on = 1;                       /* measure the far-end rail and total current now */
  /* USER CODE END 2 */

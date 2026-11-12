/* I2C displays: scan the bus once at startup. Read i2c_found[] in the debugger.
 * Paste each block into the matching USER CODE section of Core/Src/main.c. */

/* USER CODE BEGIN PV */
volatile uint8_t i2c_found[16];
volatile uint8_t i2c_found_count = 0;
/* USER CODE END PV */

  /* USER CODE BEGIN 2 */
  /* The scan runs once. Results are read in the debugger, not printed. */
  for (uint8_t addr = 0x08; addr <= 0x77; addr++) {
    if (HAL_I2C_IsDeviceReady(&hi2c1, (uint16_t)(addr << 1), 2, 10) == HAL_OK) {
      if (i2c_found_count < 16) {
        i2c_found[i2c_found_count] = addr;
      }
      i2c_found_count++;
    }
  }
  /* USER CODE END 2 */

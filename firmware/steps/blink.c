/* MCU core: blink PC13 at 1 Hz. Paste into Core/Src/main.c.
 * Verified on hardware 2026-10-11. */

  /* USER CODE BEGIN WHILE */
  while (1)
  {
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
    HAL_Delay(500);
    /* USER CODE END WHILE */

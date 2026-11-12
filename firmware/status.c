/* status.c : RGB LED on PA8 to PA10, buzzer on TIM3 CH1 (PA6).
 * CubeMX: TIM3 prescaler 71, which gives a 1 MHz timer tick from 72 MHz. */
#include "status.h"

void RGB_Set(uint8_t r, uint8_t g, uint8_t b)
{
    HAL_GPIO_WritePin(LED_PORT, LED_R_PIN, r ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_PORT, LED_G_PIN, g ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_PORT, LED_B_PIN, b ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void Buzzer_On(uint16_t freq_hz)
{
    uint32_t period;

    if (freq_hz < 16U) {
        freq_hz = 16U;                      /* keeps the period inside 16 bits */
    }
    period = 1000000UL / freq_hz;           /* timer ticks per cycle at 1 MHz */
    __HAL_TIM_SET_AUTORELOAD(&htim3, period - 1U);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, period / 2U);  /* 50 % duty */
    __HAL_TIM_SET_COUNTER(&htim3, 0);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
}

void Buzzer_Off(void)
{
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
}

void Buzzer_Beep(uint16_t freq_hz, uint16_t ms)
{
    Buzzer_On(freq_hz);
    HAL_Delay(ms);
    Buzzer_Off();
}

/* inputs.c : buttons are active low with internal pull-ups. The ADC is read
 * one channel at a time, so CubeMX stays at one conversion, scan mode off. */
#include "inputs.h"

uint8_t Button_UndoDown(void)
{
    return HAL_GPIO_ReadPin(BTN_PORT, BTN_UNDO_PIN) == GPIO_PIN_RESET;
}

uint8_t Button_NewGameDown(void)
{
    return HAL_GPIO_ReadPin(BTN_PORT, BTN_NEW_PIN) == GPIO_PIN_RESET;
}

uint16_t ADC_ReadChannel(uint32_t channel)
{
    ADC_ChannelConfTypeDef cfg = {0};
    uint16_t value = 0xFFFFU;

    cfg.Channel = channel;
    cfg.Rank = ADC_REGULAR_RANK_1;
    cfg.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;
    if (HAL_ADC_ConfigChannel(&hadc1, &cfg) != HAL_OK) {
        return value;
    }
    if (HAL_ADC_Start(&hadc1) == HAL_OK && HAL_ADC_PollForConversion(&hadc1, 5) == HAL_OK) {
        value = (uint16_t)HAL_ADC_GetValue(&hadc1);
    }
    HAL_ADC_Stop(&hadc1);
    return value;
}

uint8_t Pot_ReadBrightness(void)
{
    uint16_t raw = ADC_ReadChannel(ADC_CH_POT);
    if (raw == 0xFFFFU) {
        return 8;                           /* mid brightness if the ADC fails */
    }
    return (uint8_t)(raw >> 8);             /* 12-bit reading to 0..15 */
}

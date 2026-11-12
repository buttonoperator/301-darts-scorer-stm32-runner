/* audio.c : the timer runs a 64 kHz PWM carrier. Every fourth period the duty
 * cycle is set to the next sample of a sine wave, a 16 kHz sample rate. The RC
 * filter removes the carrier and leaves the tone.
 * CubeMX: TIM4 prescaler 0, CH3 PWM Generation, TIM4 global interrupt enabled. */
#include "audio.h"

#define CARRIER_ARR     1124U       /* 72 MHz / 1125 = 64 kHz */
#define DUTY_MID        562U        /* half scale, the silent level */
#define DECIMATE        4U          /* 64 kHz / 4 = 16 kHz sample rate */
#define SAMPLE_RATE     16000UL

static const int8_t sine32[32] = {
       0,   25,   49,   71,   90,  106,  117,  125,
     127,  125,  117,  106,   90,   71,   49,   25,
       0,  -25,  -49,  -71,  -90, -106, -117, -125,
    -127, -125, -117, -106,  -90,  -71,  -49,  -25
};

static volatile uint32_t phase_inc = 0;     /* 0 means silent */
static volatile uint8_t  vol = 0;
static uint32_t phase = 0;
static uint8_t  decim = 0;

void Audio_Init(void)
{
    __HAL_TIM_SET_AUTORELOAD(&htim4, CARRIER_ARR);
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, DUTY_MID);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
    __HAL_TIM_ENABLE_IT(&htim4, TIM_IT_UPDATE);
}

void Audio_Tone(uint16_t freq_hz, uint8_t volume)
{
    if (volume > AUDIO_VOL_MAX) {
        volume = AUDIO_VOL_MAX;
    }
    vol = volume;
    phase_inc = (uint32_t)(((uint64_t)freq_hz << 32) / SAMPLE_RATE);
}

void Audio_Stop(void)
{
    phase_inc = 0;
}

void Audio_TimerISR(void)
{
    int32_t s;

    if (++decim < DECIMATE) {
        return;
    }
    decim = 0;
    if (phase_inc == 0U) {
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, DUTY_MID);
        return;
    }
    phase += phase_inc;
    s = (int32_t)sine32[phase >> 27];               /* top 5 bits index the table */
    s = (s * (int32_t)vol * (int32_t)DUTY_MID) / (127 * 255);
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, (uint32_t)((int32_t)DUTY_MID + s));
}

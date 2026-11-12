/* audio.h : PWM audio on TIM4 CH3 (PB8) into the RC filter and PAM8403. */
#ifndef AUDIO_H
#define AUDIO_H

#include "board.h"

/* Firmware volume cap, of 255. The PAM8403 is rated 3 W and the speaker 0.5 W.
 * Raise this only while measuring the 5 V rail and listening for distortion. */
#define AUDIO_VOL_MAX   48U

void Audio_Init(void);                              /* start the PWM carrier */
void Audio_Tone(uint16_t freq_hz, uint8_t volume);  /* volume is clamped to AUDIO_VOL_MAX */
void Audio_Stop(void);
void Audio_TimerISR(void);                          /* call from the TIM4 update interrupt */

#endif /* AUDIO_H */

/* status.h : RGB status LED and passive buzzer. */
#ifndef STATUS_H
#define STATUS_H

#include "board.h"

void RGB_Set(uint8_t r, uint8_t g, uint8_t b);      /* each 0 or 1 */
void Buzzer_On(uint16_t freq_hz);                   /* 16 Hz and up */
void Buzzer_Off(void);
void Buzzer_Beep(uint16_t freq_hz, uint16_t ms);    /* blocking, for bring-up tests */

#endif /* STATUS_H */

/* inputs.h : Undo and New Game buttons, brightness pot. */
#ifndef INPUTS_H
#define INPUTS_H

#include "board.h"

uint8_t  Button_UndoDown(void);         /* 1 while pressed */
uint8_t  Button_NewGameDown(void);      /* 1 while pressed */
uint16_t ADC_ReadChannel(uint32_t channel);   /* 0 to 4095, 0xFFFF on timeout */
uint8_t  Pot_ReadBrightness(void);      /* 0 to 15, for HT16K33_SetBrightness */

#endif /* INPUTS_H */

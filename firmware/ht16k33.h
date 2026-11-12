/* ht16k33.h : Adafruit 1.2 in 4-digit 7-segment backpack (HT16K33) on I2C1. */
#ifndef HT16K33_H
#define HT16K33_H

#include "board.h"

/* Each call takes the 7-bit address, DISP1_ADDR or DISP2_ADDR. */
HAL_StatusTypeDef HT16K33_Init(uint8_t addr);                    /* oscillator on, display on, mid brightness */
HAL_StatusTypeDef HT16K33_SetBrightness(uint8_t addr, uint8_t level); /* 0 to 15 */
HAL_StatusTypeDef HT16K33_AllSegments(uint8_t addr, uint8_t on); /* full-segment test or blank */
HAL_StatusTypeDef HT16K33_ShowNumber(uint8_t addr, uint16_t value); /* 0 to 9999, leading digits blank */

#endif /* HT16K33_H */

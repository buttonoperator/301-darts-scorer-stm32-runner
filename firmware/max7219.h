/* max7219.h : two chains of MAX7219 8x8 matrix modules on SPI2, through the 74HCT541.
 * Module 0 is the module wired to the chain input (nearest the MCU). */
#ifndef MAX7219_H
#define MAX7219_H

#include "board.h"

#define MX_CHAIN_A  0U
#define MX_CHAIN_B  1U

HAL_StatusTypeDef MX_Init(uint8_t chain);                        /* shutdown off, decode off, scan 8, intensity at cap, blank */
HAL_StatusTypeDef MX_SetIntensity(uint8_t chain, uint8_t level); /* 0 to 15, clamped to MX_INTENSITY_CAP */
void MX_SetRow(uint8_t chain, uint8_t module, uint8_t row, uint8_t bits); /* frame buffer only */
void MX_Fill(uint8_t chain, uint8_t on);                         /* frame buffer only */
HAL_StatusTypeDef MX_Flush(uint8_t chain);                       /* write the frame buffer to the chain */

/* Measurement only. Lights every LED of every module in the chain at 31/32 duty,
 * ignoring the intensity register. One module draws up to 330 mA in this mode.
 * Use it with a single module fitted, never on a full chain. */
HAL_StatusTypeDef MX_DisplayTest(uint8_t chain, uint8_t on);

#endif /* MAX7219_H */

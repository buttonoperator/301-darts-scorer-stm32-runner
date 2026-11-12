/* ht16k33.c : HT16K33 driver for the Adafruit 4-digit 7-segment backpack.
 * Display RAM layout on this backpack: digit 0 at 0x00, digit 1 at 0x02,
 * colon and dots at 0x04, digit 2 at 0x06, digit 3 at 0x08.
 * Segment bits: bit0 = a ... bit6 = g. */
#include "ht16k33.h"

#define CMD_OSC_ON      0x21U
#define CMD_DISPLAY_ON  0x81U   /* display on, blink off */
#define CMD_BRIGHTNESS  0xE0U   /* OR with 0..15 */
#define I2C_TIMEOUT_MS  20U

static const uint8_t digit_font[10] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F
};

static HAL_StatusTypeDef send_cmd(uint8_t addr, uint8_t cmd)
{
    return HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)(addr << 1), &cmd, 1, I2C_TIMEOUT_MS);
}

/* buf[0] is the RAM start address, buf[1..16] are the 16 RAM bytes. */
static HAL_StatusTypeDef send_ram(uint8_t addr, uint8_t *buf)
{
    buf[0] = 0x00;
    return HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)(addr << 1), buf, 17, I2C_TIMEOUT_MS);
}

HAL_StatusTypeDef HT16K33_Init(uint8_t addr)
{
    HAL_StatusTypeDef st = send_cmd(addr, CMD_OSC_ON);
    if (st != HAL_OK) {
        return st;
    }
    st = HT16K33_SetBrightness(addr, 8);
    if (st != HAL_OK) {
        return st;
    }
    st = HT16K33_AllSegments(addr, 0);
    if (st != HAL_OK) {
        return st;
    }
    return send_cmd(addr, CMD_DISPLAY_ON);
}

HAL_StatusTypeDef HT16K33_SetBrightness(uint8_t addr, uint8_t level)
{
    if (level > 15U) {
        level = 15U;
    }
    return send_cmd(addr, (uint8_t)(CMD_BRIGHTNESS | level));
}

HAL_StatusTypeDef HT16K33_AllSegments(uint8_t addr, uint8_t on)
{
    uint8_t buf[17];
    for (uint8_t i = 1; i < 17U; i++) {
        buf[i] = on ? 0xFFU : 0x00U;
    }
    return send_ram(addr, buf);
}

HAL_StatusTypeDef HT16K33_ShowNumber(uint8_t addr, uint16_t value)
{
    static const uint8_t pos[4] = {1, 3, 7, 9}; /* buffer index of each digit, left to right */
    uint8_t buf[17] = {0};

    if (value > 9999U) {
        value = 9999U;
    }
    for (int8_t d = 3; d >= 0; d--) {
        buf[pos[d]] = digit_font[value % 10U];
        value /= 10U;
        if (value == 0U) {
            break; /* leave the leading digits blank */
        }
    }
    return send_ram(addr, buf);
}

/* lcd_i2c.c : HD44780 through a PCF8574 backpack.
 * Expander bit map used by the common blue backpack:
 *   P0 RS, P1 RW, P2 E, P3 backlight, P4 to P7 D4 to D7.
 * Confirm on the part in hand: if text never appears but the backlight
 * responds, the bit map differs. Edit the defines below.
 * RW is always written 0. The busy flag is never read; fixed delays are used. */
#include "lcd_i2c.h"

#define BIT_RS          0x01U
#define BIT_RW          0x02U
#define BIT_E           0x04U
#define BIT_BL          0x08U
#define I2C_TIMEOUT_MS  20U

/* One backlight state per possible PCF8574 / PCF8574A address. */
static uint8_t bl_state[128];

static HAL_StatusTypeDef expander_write(uint8_t addr, uint8_t v)
{
    v |= bl_state[addr & 0x7FU];
    return HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)(addr << 1), &v, 1, I2C_TIMEOUT_MS);
}

/* Present one nibble (already in bits 4 to 7) and pulse E. */
static HAL_StatusTypeDef write_nibble(uint8_t addr, uint8_t hi_nibble, uint8_t rs)
{
    uint8_t v = (uint8_t)((hi_nibble & 0xF0U) | (rs ? BIT_RS : 0U));
    HAL_StatusTypeDef st = expander_write(addr, (uint8_t)(v | BIT_E));
    if (st != HAL_OK) {
        return st;
    }
    /* One I2C byte at 100 kHz takes about 90 us, which covers the 450 ns E pulse. */
    return expander_write(addr, v);
}

static HAL_StatusTypeDef write_byte(uint8_t addr, uint8_t value, uint8_t rs)
{
    HAL_StatusTypeDef st = write_nibble(addr, (uint8_t)(value & 0xF0U), rs);
    if (st != HAL_OK) {
        return st;
    }
    st = write_nibble(addr, (uint8_t)(value << 4), rs);
    Board_DelayUs(50);                      /* most commands take 37 us */
    return st;
}

HAL_StatusTypeDef LCDI2C_Init(uint8_t addr)
{
    HAL_StatusTypeDef st;

    bl_state[addr & 0x7FU] = BIT_BL;
    HAL_Delay(50);                          /* controller power-on time */
    st = expander_write(addr, 0x00U);
    if (st != HAL_OK) {
        return st;                          /* no acknowledge: check the address */
    }

    /* Force 8-bit mode three times, then switch to 4-bit. */
    write_nibble(addr, 0x30U, 0); HAL_Delay(5);
    write_nibble(addr, 0x30U, 0); HAL_Delay(1);
    write_nibble(addr, 0x30U, 0); HAL_Delay(1);
    write_nibble(addr, 0x20U, 0); HAL_Delay(1);

    write_byte(addr, 0x28U, 0);             /* 4-bit, 2 lines, 5x8 font */
    write_byte(addr, 0x08U, 0);             /* display off */
    LCDI2C_Clear(addr);
    write_byte(addr, 0x06U, 0);             /* entry mode: increment, no shift */
    return write_byte(addr, 0x0CU, 0);      /* display on, cursor off */
}

HAL_StatusTypeDef LCDI2C_Clear(uint8_t addr)
{
    HAL_StatusTypeDef st = write_byte(addr, 0x01U, 0);
    HAL_Delay(2);                           /* clear takes 1.52 ms */
    return st;
}

HAL_StatusTypeDef LCDI2C_SetCursor(uint8_t addr, uint8_t row, uint8_t col)
{
    return write_byte(addr, (uint8_t)(0x80U | ((row ? 0x40U : 0x00U) + (col & 0x0FU))), 0);
}

HAL_StatusTypeDef LCDI2C_PutChar(uint8_t addr, char c)
{
    return write_byte(addr, (uint8_t)c, 1);
}

HAL_StatusTypeDef LCDI2C_Print(uint8_t addr, const char *s)
{
    HAL_StatusTypeDef st = HAL_OK;
    while (*s != '\0' && st == HAL_OK) {
        st = LCDI2C_PutChar(addr, *s++);
    }
    return st;
}

HAL_StatusTypeDef LCDI2C_Backlight(uint8_t addr, uint8_t on)
{
    bl_state[addr & 0x7FU] = on ? BIT_BL : 0U;
    return expander_write(addr, 0x00U);
}

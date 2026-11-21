/* max7219.c : MAX7219 chains on SPI2 (CPOL 0, CPHA 1 edge, MSB first, 8-bit).
 * Each frame is 16 bits per module: register address, then data. The first
 * 16 bits sent end up in the module farthest from the input, so a chain write
 * sends module MX_MODULES-1 first and module 0 last, then raises LOAD.
 * CLK and DIN are shared by both chains. The MAX7219 shifts on every CLK edge
 * whatever the state of LOAD, so the idle chain shifts garbage but never
 * latches it, because its LOAD line stays high. Every write therefore covers
 * all modules of one chain. */
#include "max7219.h"

#define REG_NOOP        0x00U
#define REG_DIGIT0      0x01U   /* rows 0 to 7 are registers 0x01 to 0x08 */
#define REG_DECODE      0x09U
#define REG_INTENSITY   0x0AU
#define REG_SCANLIMIT   0x0BU
#define REG_SHUTDOWN    0x0CU
#define REG_TEST        0x0FU
#define SPI_TIMEOUT_MS  10U

static uint8_t fb[2][MX_MODULES][8];

static uint16_t cs_pin(uint8_t chain)
{
    return chain == MX_CHAIN_A ? MX_CSA_PIN : MX_CSB_PIN;
}

/* Write one register in every module of the chain. data[m] goes to module m. */
static HAL_StatusTypeDef write_all(uint8_t chain, uint8_t reg, const uint8_t *data)
{
    uint8_t buf[2U * MX_MODULES];
    HAL_StatusTypeDef st;

    for (uint8_t i = 0; i < MX_MODULES; i++) {
        uint8_t m = (uint8_t)(MX_MODULES - 1U - i);     /* farthest module first */
        buf[2U * i]      = reg;
        buf[2U * i + 1U] = data[m];
    }
    HAL_GPIO_WritePin(MX_CS_PORT, cs_pin(chain), GPIO_PIN_RESET);
    st = HAL_SPI_Transmit(&hspi2, buf, (uint16_t)sizeof(buf), SPI_TIMEOUT_MS);
    HAL_GPIO_WritePin(MX_CS_PORT, cs_pin(chain), GPIO_PIN_SET);   /* rising edge latches */
    return st;
}

static HAL_StatusTypeDef write_same(uint8_t chain, uint8_t reg, uint8_t value)
{
    uint8_t data[MX_MODULES];
    for (uint8_t m = 0; m < MX_MODULES; m++) {
        data[m] = value;
    }
    return write_all(chain, reg, data);
}

HAL_StatusTypeDef MX_Init(uint8_t chain)
{
    HAL_StatusTypeDef st;

    HAL_GPIO_WritePin(MX_CS_PORT, MX_CSA_PIN | MX_CSB_PIN, GPIO_PIN_SET);
    st = write_same(chain, REG_TEST, 0x00U);            /* display test off */
    if (st != HAL_OK) {
        return st;
    }
    write_same(chain, REG_SHUTDOWN, 0x00U);             /* stay dark while configuring */
    write_same(chain, REG_DECODE, 0x00U);               /* raw matrix, no BCD decode */
    write_same(chain, REG_SCANLIMIT, 0x07U);            /* scan all 8 rows */
    write_same(chain, REG_INTENSITY, MX_INTENSITY_CAP);
    MX_Fill(chain, 0);
    st = MX_Flush(chain);
    if (st != HAL_OK) {
        return st;
    }
    return write_same(chain, REG_SHUTDOWN, 0x01U);      /* normal operation */
}

HAL_StatusTypeDef MX_SetIntensity(uint8_t chain, uint8_t level)
{
    if (level > MX_INTENSITY_CAP) {
        level = MX_INTENSITY_CAP;
    }
    return write_same(chain, REG_INTENSITY, level);
}

void MX_SetRow(uint8_t chain, uint8_t module, uint8_t row, uint8_t bits)
{
    if (chain <= MX_CHAIN_B && module < MX_MODULES && row < 8U) {
        fb[chain][module][row] = bits;
    }
}

void MX_Fill(uint8_t chain, uint8_t on)
{
    for (uint8_t m = 0; m < MX_MODULES; m++) {
        for (uint8_t r = 0; r < 8U; r++) {
            fb[chain][m][r] = on ? 0xFFU : 0x00U;
        }
    }
}

HAL_StatusTypeDef MX_Flush(uint8_t chain)
{
    uint8_t data[MX_MODULES];
    HAL_StatusTypeDef st = HAL_OK;

    for (uint8_t r = 0; r < 8U && st == HAL_OK; r++) {
        for (uint8_t m = 0; m < MX_MODULES; m++) {
            data[m] = fb[chain][m][r];
        }
        st = write_all(chain, (uint8_t)(REG_DIGIT0 + r), data);
    }
    return st;
}

HAL_StatusTypeDef MX_DisplayTest(uint8_t chain, uint8_t on)
{
    return write_same(chain, REG_TEST, on ? 0x01U : 0x00U);
}

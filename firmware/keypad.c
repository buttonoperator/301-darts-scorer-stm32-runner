/* keypad.c : matrix scan. Rows idle high. One row is driven low at a time and
 * a pressed key pulls its column low against the internal pull-up.
 * PA15, PB3 and PB4 need SYS Debug = Serial Wire in CubeMX. */
#include "keypad.h"

static const uint16_t row_pin[4] = {KEY_R1_PIN, KEY_R2_PIN, KEY_R3_PIN, KEY_R4_PIN};
static const uint16_t col_pin[4] = {KEY_C1_PIN, KEY_C2_PIN, KEY_C3_PIN, KEY_C4_PIN};

/* Edit this table if the keypad legend differs. */
static const char key_map[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

char Keypad_Scan(void)
{
    char found = 0;

    for (uint8_t r = 0; r < 4U; r++) {
        HAL_GPIO_WritePin(KEY_ROW_PORT, row_pin[r], GPIO_PIN_RESET);
        Board_DelayUs(20);                  /* let the column lines settle */
        for (uint8_t c = 0; c < 4U; c++) {
            if (HAL_GPIO_ReadPin(KEY_COL_PORT, col_pin[c]) == GPIO_PIN_RESET && found == 0) {
                found = key_map[r][c];
            }
        }
        HAL_GPIO_WritePin(KEY_ROW_PORT, row_pin[r], GPIO_PIN_SET);
    }
    return found;
}

char Keypad_GetKey(void)
{
    static uint8_t held = 0;
    static uint32_t last_event = 0;
    char k = Keypad_Scan();
    uint32_t now = HAL_GetTick();

    if (k == 0) {
        held = 0;
        return 0;
    }
    if (held == 0U && (now - last_event) >= KEYPAD_DEBOUNCE_MS) {
        held = 1;
        last_event = now;
        return k;
    }
    return 0;
}

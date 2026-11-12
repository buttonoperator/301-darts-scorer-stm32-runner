/* board.c : shared helpers. */
#include "board.h"

void Board_DelayUs(uint32_t us)
{
    /* About 6 to 10 CPU cycles per pass, so 12 passes cover 1 us at 72 MHz
     * at any optimization level. Longer than requested is acceptable here. */
    for (volatile uint32_t n = us * 12U; n != 0U; n--) {
    }
}

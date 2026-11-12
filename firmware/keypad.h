/* keypad.h : 4x4 matrix keypad. Rows driven, columns read. */
#ifndef KEYPAD_H
#define KEYPAD_H

#include "board.h"

#define KEYPAD_DEBOUNCE_MS 300U     /* project requirement */

char Keypad_Scan(void);     /* key held right now, 0 if none */
char Keypad_GetKey(void);   /* one event per press, 0 otherwise. Call every few ms. */

#endif /* KEYPAD_H */

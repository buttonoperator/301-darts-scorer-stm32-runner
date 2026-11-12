# 301 Darts Scorer: STM32 Build Runner

An interactive, single-file bring-up guide for a breadboard darts scorer: an STM32F103C8T6
Blue Pill, two HT16K33 seven-segment backpacks on I2C, an LCD1602 behind a 74HCT541 buffer,
a 4x4 keypad with LED lighting, an RGB status LED, a passive buzzer, and PWM audio through a
PAM8403 amplifier. It scores 301 or 501 for one to four players.

### ▶ [Open the build runner](https://buttonoperator.github.io/301-darts-scorer-stm32-runner/)

One HTML file, no dependencies at run time. Clone the repo and open [`index.html`](index.html)
directly to use it at the bench without a network.

---

## What the page does

- Walks 30 steps in ten stages, one subsystem at a time. Each stage ends in a measured pass
  criterion before the next subsystem is connected.
- Lights the pins for the current step on a pin map of every part. Selecting a pin traces
  its net across all parts.
- Shows a system diagram with the current stage highlighted and each block's supply rail.
- Shows the firmware for each step, with a copy button.
- Grades 5 V and 3.3 V rail readings against a 5 % tolerance.
- Saves progress in the browser.

| Stage | Subsystem | Pass criterion |
|---|---|---|
| 1 | Bench or USB power | 4.75 to 5.25 V and 3.14 to 3.47 V |
| 2 | MCU core | PC13 blinks at 1 Hz, breakpoint halts |
| 3 | I2C displays | Scan finds 0x70 and 0x71, all segments light |
| 4 | LCD | Stable text across power cycles |
| 5 | Keypad and lighting | 16 of 16 keys, one event per press |
| 6 | Buttons and pot | Active-low read, ADC sweeps 0 to 4095 |
| 7 | Status | Three LED colors, three buzzer tones |
| 8 | Audio | Clean tone, 5 V rail at or above 4.75 V |
| 9 | Battery power | Deferred. Three checkpoints before any load |
| 10 | Integration | Both rails in tolerance under full load |

## Status

| Item | State |
|---|---|
| MCU core (flash, blink, SWD debug) | Verified on hardware, 2026-10-11 |
| All other stages | Not yet verified |
| Firmware in `firmware/` | Syntax-checked on a host compiler against a stub HAL header. Not yet built with ARM GCC or run on hardware, except the blink and I2C scan logic |
| Blue Pill header order, keypad connector order | Not confirmed against the parts in hand |

The page lists the remaining open items in its last drawer.

## Repository layout

| Path | Contents |
|---|---|
| [`index.html`](index.html) | The committed single-file page |
| [`WIRING.md`](WIRING.md) | Plain-text pin map and wiring sheet |
| [`firmware/`](firmware/) | Bring-up drivers (`.c`, `.h`) for the STM32F1 HAL |
| [`firmware/steps/`](firmware/steps/) | One `main.c` test snippet per stage |
| [`src/template.html`](src/template.html) | Page source without the inlined firmware |
| [`build.py`](build.py) | Inlines `firmware/` into the template and runs checks |

## Firmware

The drivers assume a CubeMX-generated CMake project for the STM32F103C8T6 at 72 MHz, with
the peripheral handles `hi2c1`, `htim3`, `htim4` and `hadc1`. CubeMX settings are listed on
the page and in [`WIRING.md`](WIRING.md).

| File | Purpose |
|---|---|
| `board.h`, `board.c` | Pin names from the pin map, microsecond busy-wait |
| `ht16k33.h`, `ht16k33.c` | Seven-segment backpack driver on I2C1 |
| `lcd1602.h`, `lcd1602.c` | HD44780 driver, 4-bit, write-only |
| `keypad.h`, `keypad.c` | Matrix scan with a 300 ms event lockout |
| `inputs.h`, `inputs.c` | Undo and New Game buttons, ADC read, brightness pot |
| `status.h`, `status.c` | RGB LED, buzzer tones on TIM3 |
| `audio.h`, `audio.c` | Sine tone through a 64 kHz PWM carrier on TIM4, with a volume cap |

Application code goes only between `USER CODE BEGIN` and `USER CODE END` markers or in
separate source files, so CubeMX regeneration never overwrites it.

## Rebuilding

```bash
python build.py
```

Requires Python 3 and nothing else. The build inlines every file under `firmware/` into
`src/template.html`, checks that each step's code files exist and that the step count and
pin table are consistent, and writes `index.html`.

## License

MIT. See [LICENSE](LICENSE).

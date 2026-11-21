# 301 Darts Scorer: STM32 Build Runner

An interactive, single-file bring-up guide for a breadboard darts scorer powered by a 5 V adapter:
an STM32F103C8T6 Blue Pill, two HT16K33 seven-segment backpacks and two PCF8574 LCDs on one 5 V I2C
bus, two chains of eight MAX7219 8x8 matrices for Cricket marks behind a 74HCT541 buffer, a 4x4
keypad with LED lighting, an RGB status LED, a passive buzzer, and PWM audio through a PAM8403
amplifier with a switched 3.5 mm line output. It scores 301, 501 or Cricket for one to four players.
Matches project brief v4.8.

### ▶ [Open the build runner](https://buttonoperator.github.io/301-darts-scorer-stm32-runner/)

One HTML file, no dependencies at run time. Clone the repo and open [`index.html`](index.html)
directly to use it at the bench without a network.

---

## What the page does

- Walks 34 steps in nine stages, one subsystem at a time. Each stage ends in a measured pass
  criterion before the next subsystem is connected.
- Lights the pins for the current step on a pin map of every part. Selecting a pin traces
  its net across all parts.
- Shows a system diagram with the current stage highlighted and each block's supply rail.
- Shows the firmware for each step, with a copy button.
- Grades 5 V and 3.3 V rail readings against a 5 % tolerance.
- Saves progress in the browser.

| Stage | Subsystem | Pass criterion |
|---|---|---|
| 1 | Power | Adapter center positive, 4.75 to 5.25 V and 3.14 to 3.47 V |
| 2 | MCU core | PC13 blinks at 1 Hz, breakpoint halts |
| 3 | I2C bus | Pull-ups at or above 1.7 kohm total, scan finds 4 addresses, both LCDs and both displays work, display current recorded |
| 4 | Matrices | One-module current recorded, module order correct on both chains, 4.75 V or more at module 7 |
| 5 | Keypad and lighting | 16 of 16 keys, one event per press |
| 6 | Buttons and pot | Active-low read, ADC sweeps 0 to 4095 |
| 7 | Status | Three LED colors, three buzzer tones |
| 8 | Audio | Clean tone, 5 V rail at or above 4.75 V, speaker mutes with a plug in J5 |
| 9 | Integration | Both rails in tolerance under full load, current below the adapter rating |

## Status

| Item | State |
|---|---|
| MCU core (flash, blink, SWD debug) | Verified on hardware, 2026-10-11 |
| LCD backlight | Verified on hardware, 2026-11-14. Onboard 101 ohm resistor, about 22 mA |
| All other stages | Not yet verified |
| Power, I2C LCDs, matrices, 3.5 mm jack | New in brief v4.6. Not yet built |
| Firmware in `firmware/` | Syntax-checked on a host compiler against a stub HAL header. Not yet built with ARM GCC or run on hardware, except the blink and I2C scan logic |
| Blue Pill header order, AMS1117 module pin order | Read from the silkscreen, 2026-11-14 |
| Keypad connector order | Generic convention assumed. Not confirmed against the part in hand |
| LCD backpack and MAX7219 module header orders | Common-module convention assumed. Not confirmed against the parts in hand |

The page lists the remaining open items in its last drawer.

## Power

A 5 V regulated adapter on a barrel jack powers the design (brief v4.6, 2026-11-20). The earlier
USB power path is replaced because the MAX7219 matrices exceed what USB can supply.

- 5 V rail: barrel jack J4 through fuse F1 to terminal block TB1. TB1 feeds the breadboard rails
  and each matrix chain on its own 22 AWG pair.
- Blue Pill: fed from the 5 V rail through D6, a 1N5819, so a USB cable can stay plugged in for
  data without back-feeding the computer.
- 3.3 V rail: AMS1117-3.3 module, for the ESP32 header only. The Blue Pill 3V3 pin feeds the
  brightness pot only and is never joined to the 3.3 V rail.
- Firmware caps the MAX7219 intensity at 0x05. Worst case with the cap is about 2.3 A.
  Use an adapter rated 4 A or more.

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
the peripheral handles `hi2c1`, `hspi2`, `htim3`, `htim4` and `hadc1`. CubeMX settings are listed on
the page and in [`WIRING.md`](WIRING.md).

| File | Purpose |
|---|---|
| `board.h`, `board.c` | Pin names from the pin map, microsecond busy-wait |
| `ht16k33.h`, `ht16k33.c` | Seven-segment backpack driver on I2C1 |
| `lcd_i2c.h`, `lcd_i2c.c` | HD44780 through a PCF8574 I2C backpack, two LCDs by address |
| `max7219.h`, `max7219.c` | Two MAX7219 chains on SPI2 with a frame buffer and an intensity cap |
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

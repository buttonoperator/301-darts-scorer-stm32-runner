# 301 Darts Scorer: Wiring Sheet

STM32F103C8T6 Blue Pill breadboard prototype. Pin assignments are pin map v1, 2026-10-11.
Logic is 3.3 V. The LCD and its buffer, the audio amplifier and the keypad lighting run from
the 5 V rail.

Verification state: PA13, PA14 and PC13 are verified on hardware. Every other connection is
unverified until its bring-up step passes. The Blue Pill header order and the keypad
connector order are not confirmed against the parts in hand.

## 1. Power

USB through the Blue Pill connector. Source rated 1 A or more.

| From | To | Notes |
|---|---|---|
| Blue Pill 5V pin (VBUS) | F1 pin 1 | F1 is a 1 A resettable fuse |
| F1 pin 2 | 5 V rail | |
| 5 V rail | AMS1117-3.3 module IN | Module has a 3-pin header. Pin order not confirmed |
| AMS1117 OUT | 3.3 V rail | |
| Blue Pill GND, AMS1117 GND | GND rail | All grounds common |
| Blue Pill 3V3 pin | RV1 only | Never joined to the 3.3 V rail |

Two regulator outputs are never joined. The ST-Link 3.3 V pin stays open.

| Rail | Loads |
|---|---|
| 5 V | LCD1602 and backlight, 74HCT541, PAM8403, keypad LEDs, AMS1117 input |
| 3.3 V (AMS1117) | Both HT16K33 backpacks |
| Blue Pill 3V3 pin (onboard regulator) | MCU, brightness pot |
| MCU pins | RGB LED, buzzer |

## 2. MCU pin assignments

| Pin | Function | CubeMX mode | Configuration |
|---|---|---|---|
| PA0 | Brightness pot wiper | ADC1_IN0 | Analog |
| PA1 | Spare | None | Unused |
| PA2 | ESP link TX (reserved) | USART2_TX | Reserved |
| PA3 | ESP link RX (reserved) | USART2_RX | Reserved |
| PA4 | Keypad row 1 | GPIO_Output | Push-pull, idle high |
| PA5 | Keypad row 2 | GPIO_Output | Push-pull, idle high |
| PA6 | Passive buzzer | TIM3_CH1 | PWM |
| PA7 | Keypad row 3 | GPIO_Output | Push-pull, idle high |
| PA8 | RGB LED red | GPIO_Output | |
| PA9 | RGB LED green | GPIO_Output | |
| PA10 | RGB LED blue | GPIO_Output | |
| PA11, PA12 | USB D-, D+ (reserved) | | |
| PA13 | SWDIO | SYS | Debug = Serial Wire |
| PA14 | SWCLK | SYS | Debug = Serial Wire |
| PA15 | Keypad row 4 | GPIO_Output | Requires JTAG released |
| PB0 | Keypad column 1 | GPIO_Input | Pull-up |
| PB1 | Keypad column 2 | GPIO_Input | Pull-up |
| PB3 | Keypad column 3 | GPIO_Input | Pull-up, requires JTAG released |
| PB4 | Keypad column 4 | GPIO_Input | Pull-up, requires JTAG released |
| PB5 | Undo button | GPIO_Input | Pull-up, active low |
| PB6 | HT16K33 SCL | I2C1_SCL | 100 kHz |
| PB7 | HT16K33 SDA | I2C1_SDA | 100 kHz |
| PB8 | Speaker PWM | TIM4_CH3 | PWM |
| PB9 | New Game button | GPIO_Input | Pull-up, active low |
| PB10 | LCD RS | GPIO_Output | Push-pull, low speed |
| PB11 | LCD E | GPIO_Output | Push-pull, low speed |
| PB12 to PB15 | LCD D4 to D7 | GPIO_Output | Push-pull, low speed |
| PC13 | Onboard LED | GPIO_Output | Active low |

Unavailable: PB2 (BOOT1), PC14 and PC15 (32.768 kHz crystal), PD0 and PD1 (HSE crystal).

## 3. CubeMX settings

| Block | Setting |
|---|---|
| SYS | Debug = Serial Wire. Never No Debug |
| RCC | HSE crystal 8 MHz, PLL x9, SYSCLK 72 MHz, APB1 /2 |
| I2C1 | Standard mode, 100 kHz |
| ADC1 | IN0, prescaler /6, one conversion, scan mode off |
| TIM3 | CH1 PWM Generation, prescaler 71 |
| TIM4 | CH3 PWM Generation, prescaler 0, global interrupt enabled |
| Project | Toolchain CMake |

## 4. I2C displays

Two Adafruit 1.2 in 4-digit seven-segment backpacks (HT16K33). Display 1 at 0x70. Display 2
at 0x71 with the A0 jumper bridged. Pull-ups are on the backpacks.

| Backpack pin | Connects to |
|---|---|
| VIN | 3.3 V rail |
| GND | GND rail |
| SDA | PB7 |
| SCL | PB6 |

## 5. LCD1602 through the 74HCT541

| Signal | MCU pin | 74HCT541 input | 74HCT541 output | LCD pin |
|---|---|---|---|---|
| RS | PB10 | A1, pin 2 | Y1, pin 18 | 4 |
| E | PB11 | A2, pin 3 | Y2, pin 17 | 6 |
| D4 | PB12 | A3, pin 4 | Y3, pin 16 | 11 |
| D5 | PB13 | A4, pin 5 | Y4, pin 15 | 12 |
| D6 | PB14 | A5, pin 6 | Y5, pin 14 | 13 |
| D7 | PB15 | A6, pin 7 | Y6, pin 13 | 14 |

| Part | Pin | Connects to |
|---|---|---|
| 74HCT541 | 20 (VCC) | 5 V rail |
| 74HCT541 | 10 (GND), 1 (OE1), 19 (OE2), 8 (A7), 9 (A8) | GND rail |
| LCD | 1 (VSS), 5 (RW), 16 (K) | GND rail |
| LCD | 2 (VDD) | 5 V rail |
| LCD | 3 (V0) | RV2 wiper. RV2 ends to 5 V and GND |
| LCD | 7 to 10 (D0 to D3) | Open |
| LCD | 15 (A) | 5 V rail. The module has a 101 ohm backlight resistor onboard (measured 2026-11-14, about 22 mA) |

The buffer must be the HCT family. A 74HC541 at 5 V does not read 3.3 V as a guaranteed high.

## 6. Keypad and lighting

| Keypad lead | MCU pin |
|---|---|
| R1, R2, R3, R4 | PA4, PA5, PA7, PA15 |
| C1, C2, C3, C4 | PB0, PB1, PB3, PB4 |

Lighting: four LEDs (D2 to D5), each with its own series resistor (R7 to R10) from the 5 V
rail to the anode, cathode to GND. Always on. White LEDs with 220 ohm, about 8 to 9 mA per LED.

## 7. Buttons, pot, status

| Part | Connection |
|---|---|
| SW1 Undo | PB5 to GND |
| SW2 New Game | PB9 to GND |
| RV1 brightness pot | Ends to the Blue Pill 3V3 pin and GND, wiper to PA0 |
| D1 RGB LED, common cathode | PA8, PA9, PA10 through R1, R2, R3 (220 to 330 ohm) to the anodes. Cathode to GND |
| BZ1 passive buzzer | + to PA6, - to GND |

## 8. Audio

| From | To |
|---|---|
| PB8 | R4 (1 kohm) pin 1 |
| R4 pin 2 | Filtered node. C1 (0.1 uF) from this node to GND |
| Filtered node | C2 (0.1 to 1 uF coupling) pin 1 |
| C2 pin 2 | R5 pin 1 and R6 pin 1 (10 kohm each) |
| R5 pin 2 | PAM8403 L input |
| R6 pin 2 | PAM8403 R input |
| PAM8403 input G | GND rail |
| PAM8403 5V, GND | 5 V rail, GND rail |
| PAM8403 L+, L- | 8 ohm speaker |

The amplifier outputs are bridge-tied. Neither speaker terminal connects to ground.

## 9. Debug

| ST-Link | Blue Pill SWD header |
|---|---|
| SWDIO | SWDIO (PA13) |
| SWCLK | SWCLK (PA14) |
| GND | GND |
| 3.3 V | Open. USB powers the board |

Designators (R1 to R10, C1, C2, D1 to D5, F1, RV1, RV2, SW1, SW2, BZ1) are local to this sheet
until a schematic exists.

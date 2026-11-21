# 301 Darts Scorer: Wiring Sheet

STM32F103C8T6 Blue Pill breadboard prototype. Project brief v4.6, pin map v2, 2026-11-20.
Logic is 3.3 V at the MCU. The I2C bus runs at 5 V logic. The MAX7219 lines pass through a
74HCT541 to reach 5 V levels. Power is a 5 V regulated adapter on a barrel jack.

Verification state: PA13, PA14 and PC13 are verified on hardware (on USB power, 2026-10-11).
Every other connection is unverified until its bring-up step passes. The Blue Pill header order
and AMS1117 pin order were read from the silkscreen on 2026-11-14. The keypad connector order,
the LCD backpack header order and the MAX7219 module header order are not confirmed against the
parts in hand.

## 1. Power

5 V regulated adapter, 4 A minimum (5 A recommended), 5.5 x 2.1 mm, center positive.

| From | To | Notes |
|---|---|---|
| J4 center (+) | F1 pin 1 | F1 re-selection open. MF-R300 (3.0 A hold) is the candidate |
| F1 pin 2 | TB1 +5V | TB1 is a screw-terminal block, the single distribution point |
| J4 sleeve (-) | TB1 GND | |
| TB1 +5V, TB1 GND | Breadboard 5 V and GND rails | 22 AWG pair |
| TB1 +5V, TB1 GND | Matrix chain A module 0 VCC, GND | Own 22 AWG pair. C4, 470 to 1000 uF, at the module input |
| TB1 +5V, TB1 GND | Matrix chain B module 0 VCC, GND | Own 22 AWG pair. C5, 470 to 1000 uF, at the module input |
| 5 V rail | D6 anode | D6 is a 1N5819 |
| D6 cathode (band) | Blue Pill 5V pin | Net BP_5V. Keeps the adapter from back-feeding a computer over USB |
| 5 V rail | AMS1117-3.3 module VIN | Pins VIN, OUT, GND left to right, component side facing, pins down |
| AMS1117 OUT | 3.3 V rail | Supplies the ESP32 header only |
| Blue Pill GND, AMS1117 GND | GND rail | All grounds common |
| Blue Pill 3V3 pin | RV1 only | Never joined to the 3.3 V rail |

Two regulator outputs are never joined. The ST-Link 3.3 V pin stays open. The Blue Pill USB
connector carries data only.

| Rail | Loads |
|---|---|
| 5 V (TB1) | MAX7219 chains, both HT16K33 backpacks (VIN and VIO), both I2C LCDs, 74HCT541, PAM8403, keypad LEDs, AMS1117 input, D6 |
| 3.3 V (AMS1117) | ESP32 link header (reserved) |
| Blue Pill 3V3 pin (onboard regulator) | MCU, brightness pot |
| MCU pins | RGB LED, buzzer |

Firmware caps the MAX7219 intensity at 0x05 until one module is measured. Worst case with the cap
is about 2.3 A for the board.

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
| PA11, PA12 | USB D-, D+ (reserved) | | Data only |
| PA13 | SWDIO | SYS | Debug = Serial Wire |
| PA14 | SWCLK | SYS | Debug = Serial Wire |
| PA15 | Keypad row 4 | GPIO_Output | Requires JTAG released |
| PB0 | Keypad column 1 | GPIO_Input | Pull-up |
| PB1 | Keypad column 2 | GPIO_Input | Pull-up |
| PB3 | Keypad column 3 | GPIO_Input | Pull-up, requires JTAG released |
| PB4 | Keypad column 4 | GPIO_Input | Pull-up, requires JTAG released |
| PB5 | Undo button | GPIO_Input | Pull-up, active low |
| PB6 | I2C SCL, 4 devices | I2C1_SCL | 100 kHz, bus pulled to 5 V |
| PB7 | I2C SDA, 4 devices | I2C1_SDA | 100 kHz, bus pulled to 5 V |
| PB8 | Speaker PWM | TIM4_CH3 | PWM |
| PB9 | New Game button | GPIO_Input | Pull-up, active low |
| PB10 | DFPlayer TX (reserved) | USART3_TX | Reserved |
| PB11 | DFPlayer RX (reserved) | USART3_RX | Reserved |
| PB12 | Matrix chain A LOAD | GPIO_Output | Push-pull, initial high |
| PB13 | Matrix CLK | SPI2_SCK | Transmit Only Master |
| PB14 | Matrix chain B LOAD | GPIO_Output | Push-pull, initial high |
| PB15 | Matrix DIN | SPI2_MOSI | Transmit Only Master |
| PC13 | Onboard LED | GPIO_Output | Active low |

Unavailable: PB2 (BOOT1), PC14 and PC15 (32.768 kHz crystal), PD0 and PD1 (HSE crystal).

## 3. CubeMX settings

| Block | Setting |
|---|---|
| SYS | Debug = Serial Wire. Never No Debug |
| RCC | HSE crystal 8 MHz, PLL x9, SYSCLK 72 MHz, APB1 /2 |
| I2C1 | Standard mode, 100 kHz |
| SPI2 | Transmit Only Master, 8-bit, MSB first, CPOL low, CPHA 1 edge, prescaler /16 (2.25 MHz), NSS software |
| GPIO | PB12, PB14 output push-pull, initial level high |
| ADC1 | IN0, prescaler /6, one conversion, scan mode off |
| TIM3 | CH1 PWM Generation, prescaler 71 |
| TIM4 | CH3 PWM Generation, prescaler 0, global interrupt enabled |
| Project | Toolchain CMake |

## 4. I2C bus (5 V logic)

| Device | Address | Supply | Onboard pull-ups |
|---|---|---|---|
| DS1, HT16K33 backpack | 0x70 | VIN and VIO on 5 V | 10 kohm |
| DS2, HT16K33 backpack | 0x71, A0 bridged | VIN and VIO on 5 V | 10 kohm |
| LCD A, PCF8574 backpack | 0x27 (PCF8574) or 0x3F (PCF8574A) | VCC on 5 V | Measure |
| LCD B, PCF8574 backpack | LCD A minus 1, A0 bridged | VCC on 5 V | Measure |

| Module pin | Connects to |
|---|---|
| SDA (all four) | PB7 |
| SCL (all four) | PB6 |
| HT16K33 VIN, VIO | 5 V rail |
| LCD backpack VCC | 5 V rail |
| GND (all four) | GND rail |

The PCF8574 needs a high of at least 0.7 x VCC (3.5 V on 5 V). Pull the bus to 5 V only. PB6 and
PB7 are 5 V tolerant. Total pull-up on each line must stay at or above about 1.7 kohm. Two 10 kohm
plus two 4.7 kohm gives about 1.9 kohm.

## 5. MAX7219 matrices through the 74HCT541

| Signal | MCU pin | 74HCT541 input | 74HCT541 output | Destination |
|---|---|---|---|---|
| CLK | PB13 | A1, pin 2 | Y1, pin 18 | Chain A and chain B module 0 CLK |
| DIN | PB15 | A2, pin 3 | Y2, pin 17 | Chain A and chain B module 0 DIN |
| LOAD A | PB12 | A3, pin 4 | Y3, pin 16 | Chain A module 0 CS |
| LOAD B | PB14 | A4, pin 5 | Y4, pin 15 | Chain B module 0 CS |

| Part | Pin | Connects to |
|---|---|---|
| 74HCT541 | 20 (VCC) | 5 V rail. C3 (0.1 uF) from pin 20 to GND at the pin |
| 74HCT541 | 10 (GND), 1 (OE1), 19 (OE2), 6 to 9 (A5 to A8) | GND rail |
| 74HCT541 | 11 to 14 (Y8 to Y5) | Open |

Module input header order assumed VCC, GND, DIN, CS, CLK. Confirm on the part in hand. Modules 1
to 7 join through each module's own output-to-input header. Module 0 is the one wired to the
buffer.

The buffer must be the HCT family. The MAX7219 needs a high of at least 3.5 V.

## 6. Keypad and lighting

| Keypad lead | MCU pin |
|---|---|
| R1, R2, R3, R4 (tail pins 1 to 4, generic convention) | PA4, PA5, PA7, PA15 |
| C1, C2, C3, C4 (tail pins 5 to 8, generic convention) | PB0, PB1, PB3, PB4 |

The tail order is not confirmed on the keypad in hand. All eight lines are GPIO, so a different order is a firmware change.

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

## 8. Audio and 3.5 mm line output

| From | To |
|---|---|
| PB8 | R4 (1 kohm) pin 1 |
| R4 pin 2 | Filtered node. C1 (0.1 uF, KEMET C320C104K5R5TA) from this node to GND |
| Filtered node | C2 (1 uF, KEMET C330C105K5R5TA) pin 1 |
| C2 pin 2 (AUD_AC) | J5 tip and J5 ring |
| J5 tip switch, normally closed (AUD_SW) | R5 pin 1 and R6 pin 1 (10 kohm each), and R12 (47 kohm) to GND |
| J5 sleeve | GND rail |
| R5 pin 2 | PAM8403 L input |
| R6 pin 2 | PAM8403 R input |
| PAM8403 input G | GND rail |
| PAM8403 5V, GND | 5 V rail, GND rail |
| PAM8403 L+, L- | 8 ohm speaker |

J5 is a switched 3.5 mm jack (CUI SJ1-3524N, tip switch). Terminal numbers vary by model: identify tip,
ring, sleeve and the tip switch by continuity. The tip switch is closed to the tip with no plug, so
the signal reaches the amplifier. A plug opens it, which mutes the speaker and leaves the signal on
the plug. The output is line level, not specified for 32 ohm headphones.

The amplifier outputs are bridge-tied. Neither speaker terminal connects to ground, and the jack
never connects to the amplifier outputs.

## 9. Debug

| ST-Link | Blue Pill SWD header |
|---|---|
| SWDIO | SWDIO (PA13) |
| SWCLK | SWCLK (PA14) |
| GND | GND |
| 3.3 V | Open. The adapter powers the board |

Designators follow Altium BOM v11.

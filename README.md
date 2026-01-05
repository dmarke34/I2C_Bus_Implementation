# I2C_Bus_Implementation
## Project Overview
An embedded system combining real-time clock (RTC), temperature sensing, IR remote control decoding, and graphical display on a PIC18F4620 microcontroller. The system displays time/temperature and responds to IR remote commands with visual/audio feedback.

## Features 
- Real-Time Clock Display: DS3231 RTC for accurate timekeeping
- Temperature Monitoring: DS1621 sensor with Celsius/Fahrenheit conversion
- IR Remote Control: NEC protocol decoding for 21-button remote
- Visual Feedback: ST7735 TFT display with button-specific colors
- Audio Feedback: PWM-driven buzzer for button press confirmation
- LED Temperature Indicator: 7-color LED output based on temperature ranges
- UART Debug Interface: Serial output for monitoring system status

## Project Structure
- main.c              #Main application logic
- I2C.h / I2C.c       #I2C communication library
- I2C_Support.h       #I2C helper functions
- Interrupt.h         #Interrupt configuration
- ST7735.h            #TFT display driver
- DS1621_Support.c    #Temperature sensor interface
- DS3231_Support.c    #RTC interface
- NEC_Decoder.c       #IR remote decoding logic

# 🎮 IR Remote Button Mapping

| Button | HEX Code | Display Text | Color | Function | Notes |
|--------|----------|--------------|-------|----------|-------|
| CH- | 0xA2 | "CH-" | 🔴 Red | Channel Down | Standard remote function |
| CH0 | 0x62 | "CH0" | 🔴 Red | Channel Zero | Returns to channel 0 |
| CH+ | 0xE2 | "CH+" | 🔴 Red | Channel Up | Increments channel |
| PREV | 0x22 | "PRV" | 🔵 Blue | Previous | Navigates to previous item |
| NEXT | 0x02 | "NXT" | 🔵 Blue | Next | Navigates to next item |
| PLAY/PAUSE | 0xC2 | "P/P" | 🟢 Green | Play/Pause | Media control |
| VOL- | 0xE0 | "VL-" | 🟣 Magenta | Volume Down | Decreases volume |
| VOL+ | 0xA8 | "VL+" | 🟣 Magenta | Volume Up | Increases volume |
| **EQ** | **0x90** | **"EQ0"** | **🟣 Magenta** | **Time Reset** | **Special: Resets RTC time** |
| 0 | 0x68 | "#00" | ⚫ Black | Number 0 | Numeric input |
| 1 | 0x98 | "#01" | ⚫ Black | Number 1 | Numeric input |
| 2 | 0xB0 | "#02" | ⚫ Black | Number 2 | Numeric input |
| 3 | 0x30 | "#03" | ⚫ Black | Number 3 | Numeric input |
| 4 | 0x18 | "#04" | ⚫ Black | Number 4 | Numeric input |
| 5 | 0x7A | "#05" | ⚫ Black | Number 5 | Numeric input |
| 6 | 0x10 | "#06" | ⚫ Black | Number 6 | Numeric input |
| 7 | 0x38 | "#07" | ⚫ Black | Number 7 | Numeric input |
| 8 | 0x5A | "#08" | ⚫ Black | Number 8 | Numeric input |
| 9 | 0x42 | "#09" | ⚫ Black | Number 9 | Numeric input |

TEMPERATURE SCALE VISUALIZATION
┌─────────────────────────────────────────────────────────────────────────┐
│  COLD         COOL         MILD         WARM         HOT         EXTREME│
├────────┬──────────┬──────────┬──────────┬──────────┬──────────┬─────────┤
│ <10°F  │ 10-19°F  │ 20-29°F  │ 30-39°F  │ 40-49°F  │ 50-59°F  │ 60+°F   │
├────────┼──────────┼──────────┼──────────┼──────────┼──────────┼─────────┤
│        │          │          │          │          │          │         │
│   ⚫    │    🔴    │    🟢    │    🟡    │    🔵    │    🟣    │   ⚪    │
│   OFF  │   RED    │  GREEN   │  YELLOW  │   BLUE   │  PURPLE  │  WHITE  │
│        │          │          │          │          │          │         │
└────────┴──────────┴──────────┴──────────┴──────────┴──────────┴─────────┘

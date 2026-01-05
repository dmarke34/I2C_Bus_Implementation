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
project/
├── main.c              # Main application logic
├── I2C.h / I2C.c       # I2C communication library
├── I2C_Support.h       # I2C helper functions
├── Interrupt.h         # Interrupt configuration
├── ST7735.h            # TFT display driver
├── DS1621_Support.c    # Temperature sensor interface
├── DS3231_Support.c    # RTC interface
└── NEC_Decoder.c       # IR remote decoding logic

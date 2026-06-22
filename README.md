# CLOUD-ENABLED-IOT-THERMAL-ALERT-AND-LOGGING-SYSTEM-
This System using an LPC2148 microcontroller. The system continuously monitors temperature via an LM35 sensor, displays it on an LCD, logs data to ThingSpeak cloud over Wi-Fi, and triggers buzzer alerts when temperature crosses a configured setpoint. The set point can be updated locally via keypad and remotely through the cloud
# Cloud-Enabled IoT Thermal Alert and Logging System

A real-time temperature monitoring system built on the LPC2148 ARM7 microcontroller. It reads temperature from an LM35 sensor, displays it on a 16x2 LCD, and logs data to the ThingSpeak cloud via an ESP01 Wi-Fi module every 2 minutes.
## Built With
- Microcontroller: LPC2148 (ARM7TDMI-S)
- IDE: Keil IDE (uVision)
- Language: Embedded C (Bare-metal)
- Cloud: ThingSpeak IoT Platform
- Wi-Fi: ESP01 (ESP8266) via AT Commands

## Features
- Real-time temperature reading via LM35 and ADC
- 16x2 LCD display with time and temperature
- 4x4 Keypad for manual setpoint entry by raising interrupt.
- Setpoint stored in I2C EEPROM (AT24C256)
- Temperature data uploaded to ThingSpeak every 2 minutes
- Reads updated setpoint back from cloud which can be set remotely from cloud by user through their mobile using api keys.
- GPIO alert buzzer output when temperature exceeds setpoint
- On-chip RTC for real-time clock display

## Project Structure

```
├── cloud_main.c              # Main application loop
├── cloud_thermal.c           # All function definitions
├── cloud_thermal_defines.h   # Macros and defines
├── cloud_fun_declarations.h  # All peripheral function declarations
├── init_devices.c / .h       # Peripheral initialization
├── esp01.c / .h              # ESP01 Wi-Fi AT command driver
├── ext_interrupt_init.c / .h # External interrupts and ISRs
└── headers.h                 # Global variables

## Getting Started

1. Clone the repo and open the project in Keil uVision
2. Update Wi-Fi credentials in 'esp01.c':
   UART_TxString("AT+CWJAP=\"YourSSID\",\"YourPassword\"\r\n");
3. Add your ThingSpeak API keys in 'esp01.c'
4. Build the project (F7) and flash to LPC2148

## Cloud Setup
1. Create a free account at https://thingspeak.com
2. Create two channels - one for temperature, one for setpoint
3. Copy the API keys into `esp01.c`

## Hardware Connections

| Component     | Interface | Pins              |
|---------------|-----------|-------------------|
| LM35 Sensor   | ADC       | ADC Channel       |
| 16x2 LCD      | GPIO      | Port 0            |
| 4x4 Keypad    | GPIO      | P1.16 to P1.23    |
| EEPROM 24C256 | I2C       | P0.3 (SDA), P0.2 (SCL) |
| ESP01 Wi-Fi   | UART      | UART0 TX/RX       |
| Alert Output  | GPIO      | P0.16             |

---

## Acknowledgements

- ThingSpeak - IoT cloud platform
- Keil IDE - ARM development tools
- NXP LPC2148 Datasheet and Reference Manual


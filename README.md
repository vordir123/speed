# IoT Firmware and Dashboard

This repository contains a demonstration IoT system built with an ESP32 and a
Teensy 4.1 microcontroller. Features include pin control, a simple machine
learning model for CAN message identification, a web dashboard, and an
automation script for building and flashing firmware.

## Structure

```
firmware/
  esp32/   – ESP32 DOIT DevKit v1 firmware (Arduino + Async Web Server)
  teensy/  – Teensy 4.1 firmware with CAN bus and ML model
web/        – Web dashboard served locally
flash.sh    – Helper script for dependency installation, build and upload
```

## Usage

1. **Install dependencies and compile**
   ```bash
   ./flash.sh --compile
   ```
2. **Upload to boards**
   ```bash
   ./flash.sh --upload
   ```
3. **Open `web/index.html`** in a browser that supports the Web Serial API.
   Use the interface to control pins on both microcontrollers and view serial
   output in real time.

WiFi credentials for the ESP32 are defined in
`firmware/esp32/src/main.cpp` and should be adjusted before building.

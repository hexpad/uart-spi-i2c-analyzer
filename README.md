# Multi Protocol Analyzer

This project demonstrates how UART, SPI, and I2C communication protocols work, both on real hardware (ESP32) and in software (C++).

---

## What does it do?

- Two ESP32 boards communicate with each other using UART, SPI, and I2C
- A C++ program simulates and parses raw bit streams for the same three protocols

---

## Files

| File | Description |
|------|-------------|
| `transmitter.ino` | Sends "Wake" (UART), "Up" (SPI), "Neo" (I2C) every 5 seconds |
| `receiver.ino` | Receives the data and prints it to Serial Monitor |
| `main.cpp` | Parses raw bit arrays and decodes the messages in software |

---

## How to run

### C++

1. Compile & Run

### ESP32

1. Flash `transmitter.ino` to the first ESP32
2. Flash `receiver.ino` to the second ESP32
3. Open Serial Monitor at 115200 baud

---

## Pin connections

| Protocol | Pin |
|----------|-----|
| UART TX/RX | 17 / 16 |
| SPI CLK/MOSI/MISO/CS | 18 / 23 / 19 / 5 |
| I2C SDA/SCL | 21 / 22 |

---

## Expected output (C++)

```
[UART Analyzer] : Wake
[SPI Analyzer] : Up
[I2C Analyzer] Decoded (Addr 0x50): Neo
```

---

# Source Code

https://github.com/hexpad/uart-spi-i2c-analyzer


# Project Overview

This repository is a **hands-on STM32 firmware learning project** focused on building an **RFID / RF card reader system**.

The goal is to gain **practical, low-level experience** with STM32 microcontrollers through **register-level C programming**, working directly with peripherals on real hardware.

The project is developed **incrementally**, starting from basic peripheral drivers and gradually integrating them into a complete RF reader application.

This repository represents an **ongoing learning process**, not a finished product.

---

# Project Direction

The main objective is to implement an **RF card reader** using an external RF module (e.g. RC522), fully controlled by STM32 firmware.

The final system is expected to:

* Communicate with an RF module over **SPI**
* Detect RF cards/tags and read their **UID**
* Display card or system information
* Demonstrate system-level integration of multiple peripherals

---

# Project Structure

* **Src**        -> Source files (.c)
* **Inc**        -> Header files (.h)
* **Documents**  -> Media and documentation

---

# Status

In Progress

---

# Notes

Low-level peripheral drivers (GPIO, timers, ADC, SPI, etc.) are developed as building blocks and integrated progressively into the RF reader system.
Code is intentionally kept explicit and minimal to emphasize learning and clarity.


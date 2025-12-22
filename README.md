# Project Overview

This repository is a **hands-on STM32 firmware learning project** focused on building an **RFID / RF card reader system**.

The goal is to gain **practical, low-level experience** with STM32 microcontrollers through **register-level C programming**, working directly with real hardware.

The project evolves incrementally from individual drivers into a **cohesive embedded system**.

> **It’s not a demo, it’s a system.**

The focus is on **system-level design**: modularity, clear APIs, and non-blocking time-driven execution within a simple `while(1)` main loop.
Each component is treated as a building block that integrates cleanly into the overall system.

---

# Project Direction

The objective is to implement an **RF card reader system** using an external RF module (RC522) with a **7-segment display for system and card feedback**, controlled entirely by STM32 firmware.

Planned capabilities include:

- SPI communication with the RF module  
- RF card/tag detection and **UID** reading  
- Display of card or system information
- Integration of multiple peripherals into a single system

---

# Project Structure

- **Src/**        – Source files  
- **Inc/**        – Header files  
- **Documents/** – Media and documentation  
- **tools/**     – Helper scripts  

---

# Status

**In Progress**


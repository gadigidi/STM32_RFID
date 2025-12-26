# RFID Reader System - STM32 Firmware Project

## Overview

> **It’s not a demo - it’s a system.**

This project is intentionally designed as a **system-level embedded application**, built **from scratch** using custom peripheral drivers.

The focus is on **modular firmware architecture**, **non-blocking execution**, and **event-driven design**.
Peripheral drivers (SPI, GPIO, EXTI) are implemented as independent modules, while higher-level application logic is managed using a clear **finite state machine (FSM)**.

The RFID reader (RC522) operates using **interrupt-based signaling**, allowing the main system to remain responsive.
At the same time, a **7-segment display is refreshed continuously in the background**, demonstrating concurrent task handling without blocking delays.

This approach reflects real embedded product design considerations such as timing, scalability, and clean API boundaries.

---

## Scope

- SPI communication with the RC522 RFID module  
- RF card and tag detection with **UID** reading  
- Interrupt-driven RFID event handling  
- Background 7-segment display refresh  
- Integration of multiple peripherals into a single embedded firmware system  

---

## Project Structure

- **Src/**        - Source files  
- **Inc/**        - Header files  
- **Documents/** - Media and documentation  
- **tools/**     - Helper scripts  

---

## How It Works

The system is built around a **non-blocking finite state machine (FSM)** that coordinates RFID communication and background tasks.

In the idle state, the firmware waits for an **interrupt from the RC522 module**, indicating RF activity.
Once triggered, the FSM advances through request, response, and data retrieval phases without blocking the main loop.

RFID communication is handled via SPI transactions, while interrupt flags and internal state variables ensure safe transitions between states.
At the same time, the **7-segment display is refreshed continuously in the background**, independent of RFID activity.

This event-driven approach keeps the system responsive, deterministic, and scalable as additional features are introduced.

## Status

**In Progress**

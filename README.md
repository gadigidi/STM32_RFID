# RFID Reader System - STM32 Firmware Project

## Overview

> **It’s not a demo - it’s a system.**

This project is intentionally designed as a **system-level embedded application**, built **from scratch** using custom peripheral drivers.

The focus is on **modular firmware architecture**, **non-blocking execution**, and **event-driven design**.  
Peripheral drivers (**TIMER, SPI, GPIO, EXTI**) are implemented as independent modules, while higher-level application logic is managed using a clear **finite state machine (FSM)**.

The RFID reader (RC522) operates using **interrupt-based signaling**, allowing the main system to remain responsive.  
At the same time, a **7-segment display is refreshed continuously in the background**, demonstrating concurrent task handling without blocking delays.

This approach reflects real embedded product design considerations such as timing, scalability, and clean API boundaries.

---

## System Architecture

### RFID State Machine – ISO14443A CL1 UID Flow

```mermaid
stateDiagram-v2
    RFID_PRE_IDLE --> RFID_IDLE

    RFID_IDLE --> RFID_SEND_REQA : idle_action_timeout
    RFID_SEND_REQA --> RFID_WAIT_ATQA

    RFID_WAIT_ATQA --> RFID_SEND_ANTICOL_CL1 : ATQA OK
    RFID_WAIT_ATQA --> RFID_SHOW_ERROR : ATQA error

    RFID_SEND_ANTICOL_CL1 --> RFID_WAIT_ANTICOL_CL1

    RFID_WAIT_ANTICOL_CL1 --> RFID_CALC_CRC_CL1 : anticoll OK
    RFID_WAIT_ANTICOL_CL1 --> RFID_SHOW_ERROR : anticoll error

    RFID_CALC_CRC_CL1 --> RFID_WAIT_CRC_CL1_RESULT

    RFID_WAIT_CRC_CL1_RESULT --> RFID_SEND_SELECT_CL1 : CRC OK
    RFID_WAIT_CRC_CL1_RESULT --> RFID_SHOW_ERROR : CRC error

    RFID_SEND_SELECT_CL1 --> RFID_WAIT_SAK
    RFID_WAIT_SAK --> RFID_UID_STATUS

    RFID_UID_STATUS --> RFID_SHOW_UID_TEXT : UID valid
    RFID_UID_STATUS --> RFID_SHOW_ERROR : recoverable error
    RFID_UID_STATUS --> RFID_FATAL_ERROR : fatal error

    RFID_SHOW_UID_TEXT --> RFID_SHOW_UID_DIGITS
    RFID_SHOW_UID_DIGITS --> RFID_PRE_IDLE

    RFID_SHOW_ERROR --> RFID_PRE_IDLE
```

The FSM reflects the actual firmware implementation and maps directly to the codebase.  
Each protocol phase is explicitly modeled to enable deterministic, non-blocking execution.

---

## Project Structure

- **Src/**        - Source files  
- **Inc/**        - Header files  
- **Documents/** - Media and documentation  
- **tools/**     - Helper scripts (e.g. BSRR bit-mask generation)

---

## Hardware

The RC522 module is connected to the STM32 via SPI.

The RC522 **RST** pin and **CS** pin are held high using external **10 kΩ pull-up resistors** to 3.3 V to ensure stable operation and prevent unintended resets.

The **IRQ** signal is connected to an EXTI-capable GPIO pin for interrupt-driven operation.

The 7-segment display is connected to the STM32 GPIO pins through **current-limiting resistors** for each segment to protect the LEDs and control the segment current.

---

## Status

The RC522 subsystem is operational with a non-blocking, IRQ-driven FSM.  
REQA short frames (7-bit) are transmitted correctly, and valid ATQA responses are received from ISO14443A cards.

This milestone confirms a stable RF link and correct low-level configuration, forming a solid base for incremental FSM expansion toward UID anti-collision and card selection.


# Automated Spice Dispenser

[![C++](https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)](https://cplusplus.com/)
[![Arduino](https://img.shields.io/badge/Arduino-00979D?style=for-the-badge&logo=Arduino&logoColor=white)](https://www.arduino.cc/)
[![Autodesk Fusion](https://img.shields.io/badge/Autodesk%20Fusion-0696D7?style=for-the-badge&logo=autodesk&logoColor=white)](https://www.autodesk.com/products/fusion-360)

---

## Table of Contents
1. [Overview](#overview)
2. [System Architecture](#system-architecture)
3. [Mechanical Design & Engineering](#mechanical-design--engineering)
4. [Firmware Implementation](#firmware-implementation)
5. [Future Iterations](#future-iterations)

---

## Overview

> The Automated Spice Dispenser is an embedded mechatronic system designed to deliver precise, sub-degree volumetric dispensing of culinary spices. 

Built around a master-slave microcontroller architecture, the system integrates mechanical auger-based delivery mechanisms with a finite state machine to provide a seamless user interface and highly accurate motor control.

Initially conceived as a dual-plate gravity system, the mechanical design was completely overhauled to an auger-based extrusion model. This pivot mitigated particulate jamming and vastly improved the volumetric precision of high-resistance, variable-density spices.

---

## System Architecture

The electrical architecture utilizes a decoupled Master-Slave configuration to ensure the user interface remains non-blocking during computationally heavy motor control loops.

*   **Master Node (Arduino 1 - UI & State Management):** Handles the primary Finite State Machine (FSM). It interfaces with an LCD display and push-button matrix to capture user inputs for spice selection and desired volume.
*   **Slave Node (Arduino 2 - Actuation Controller):** Dedicated strictly to motor actuation. It receives digital signal pulses from the Master Node and translates them into precise step sequences for the target motors.

## System Architecture

The electrical architecture utilizes a decoupled Master-Slave configuration to ensure the user interface remains non-blocking during computationally heavy motor control loops.

* **Master Node (Arduino 1 - UI & State Management):** Handles the primary Finite State Machine (FSM). It interfaces with an LCD display and push-button matrix to capture user inputs for spice selection and desired volume.
* **Slave Node (Arduino 2 - Actuation Controller):** Dedicated strictly to motor actuation. It receives digital signal pulses from the Master Node and translates them into precise step sequences for the target motors.

## System Architecture

The electrical architecture utilizes a decoupled Master-Slave configuration to ensure the user interface remains non-blocking during computationally heavy motor control loops.

* **Master Node (Arduino 1 - UI & State Management):** Handles the primary Finite State Machine (FSM). It interfaces with an LCD display and push-button matrix to capture user inputs for spice selection and desired volume.
* **Slave Node (Arduino 2 - Actuation Controller):** Dedicated strictly to motor actuation. It receives digital signal pulses from the Master Node and translates them into precise step sequences for the target motors.

### Architecture & Data Flow Diagram

```text
+-------------------+                  +-------------------+
|   MASTER NODE     |  Sync Pulses     |    SLAVE NODE     |
|   (Arduino 1)     |=================>|   (Arduino 2)     |
| - UI Management   |                  | - Motor Control   |
| - State Machine   |                  | - Micro-stepping  |
+-------------------+                  +-------------------+
         ^                                       |
         | I2C / GPIO                            | Step / Dir
         v                                       v
+-------------------+                  +-------------------+
|  User Interface   |                  |   Stepper Motor   |
|  (LCD & Buttons)  |                  |                   |
+-------------------+                  +-------------------+
                                                 |
                                                 | Shaft Coupling
                                                 v
                                       +-------------------+
                                       |  Auger Extruder   |
                                       |  \==\==\==\==\==\ |
                                       |   \__\__\__\__\__\|
                                       +-------------------+
                                                 | 
                                                 | Volumetric Output
                                                 v

```

## Mechanical Design & Engineering
All custom mechanical components, including the modular spice containers, covers, and the core auger mechanism, were drafted using Autodesk Fusion. The CAD workflow was optimized for macOS trackpad environments, leveraging Tinkercad-style view and interaction settings to allow for rapid, iterative geometric adjustments during the prototyping phase.
Key Mechanical Features:

- Auger-Based Extrusion: Replaced the legacy gravity-fed plates. The auger's pitch and thread depth were mathematically derived to map a specific degree of rotational actuation to a known volumetric output.
- Torque Calibration: Motor torque requirements were calculated to account for the shear friction and compaction resistance of different spice granularities.
- Loop Calibration: Physical assemblies underwent iterative sub-degree rotational calibration to ensure tight tolerances between the stepper motor shafts and the auger couplers, preventing backlash.

## Firmware Implementation
The firmware is written entirely in C++, leveraging object-oriented principles to encapsulate hardware peripherals and state logic.
Finite State Machine (FSM): The core logic operates on an FSM that transitions cleanly between IDLE, SELECT_SPICE, SELECT_VOLUME, DISPENSING, and ERROR states.
Pulse Train Communication: To maintain timing integrity, the Master triggers the Slave via a synchronized digital pulse protocol, preventing the serial buffer overhead from interfering with step timing.
Sub-Degree Stepper Control: The Slave firmware utilizes micro-stepping algorithms to actuate the stepper motors with sub-degree accuracy, translating the user's volumetric request into exact rotational coordinates.

---

## Future Iterations

While this system represents a robust standalone solution, future developmental avenues could include integrating closed-loop feedback via load cells for gravimetric (weight-based) verification, or migrating the Master-Slave architecture to a single RTOS-capable microcontroller.

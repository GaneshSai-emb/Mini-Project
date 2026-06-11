# EventBoard: RTC-Driven Message Display System

##  Overview

The **EventBoard** is an automated, real-time embedded system developed for the ARM7TDMI-based **LPC2148** microcontroller. The system functions as an intelligent digital notice board, keeping highly accurate track of time and dynamically managing scheduled contextual notifications. 

The application utilizes the LPC2148's internal **Real-Time Clock (RTC)** module to monitor current time parameters and concurrently queries environmental temperature via an analog **LM35** sensor. If a scheduled condition matches the real-time criteria, the associated information scrolls dynamically across a **16x2 character LCD**. When idle, the board scales back to show the clock and room temperature while providing a credential-protected **Admin Mode** accessed via a **4x4 matrix keypad** to modify system variables on the fly.

This project showcases clean register-level optimization, driver development, state-machine scheduling, and hardware-software modular codesign in **Embedded C**.

---

##  Architectural Objectives

- **Hardware-Software Interface Extraction:** Develop modular bare-metal drivers from scratch for the LPC2148 internal RTC, built-in Successive Approximation ADC, a 16x2 character display, and a scanned matrix keypad.
- **Deterministic Scheduling:** Design a localized, state-aware polling structure to scan events continuously against low-level RTC hardware registers without dropping user input vectors.
- **Dynamic Data Modification:** Incorporate a robust data handling structure allowing real-time modification of scheduled memory states using structured software boundaries.
- **State-Driven Indicators:** Implement visual system-status indications via discrete hardware lines representing active warning states vs. system-idle operations.

---

## 🛠️ System Hardware Architecture & Pin Interfacing

The project is designed to explicitly utilize on-chip peripherals and standard embedded system components:

- **Microcontroller:** NXP LPC2148 (ARM7TDMI-S Core, 512KB On-Chip Flash, 32KB SRAM).
- **Core Timekeeping:** On-Chip Hardware RTC driven by an external 32.768 kHz oscillator.
- **Environmental Sensor:** Analog LM35 Temperature Sensor (Linear Scale factor: 10 mV/°C).
- **Data Input Interfaces:** 4x4 Matrix Keypad mapped to custom GPIO lines for menu driving + Dedicated Hardware Switch for Admin Mode execution toggle.
- **Display Interfacing:** 16x2 Character LCD operating over an optimized multi-bit data bus interface.
- **Acoustic & Visual Status Flags:** Dual-LED State Architecture (Green LED indicating Active Message Event; Red LED indicating Idle Real-Time Operation) accompanied by a Piezo Buzzer for physical notification feedback.

---

## Technical Specifications & Software Stack

- **Firmware Language:** Pure ISO/IEC Embedded C
- **Development Toolchain:** Keil uVision IDE v5
- **Compilation Target:** ARM CC (Compiler v5 for ARM7 microcontrollers)
- **Programming Hardware:** Flash Magic tool via In-System Programming (ISP) UART0 communication.

---

## ⚙️ Core Working Principle & Hardware Flow

The architecture operates within a highly systematic execution loop divided into distinct hardware phases:

1. **System Initialization:** On boot, the system initializes system clocks (CCLK = 60 MHz, PCLK = 15 MHz). The custom configuration sets pin selection control arrays (`PINSEL1`), establishes data direction control lines via `IODIR1` for matrix operations, and prepares custom clock dividers for accurate peripheral clock routing.
2. **Environmental Tracking Phase:** The system periodically triggers conversions over the Successive Approximation ADC block channel 1 (`CH1`). The conversion evaluates the 0V to 3.3V analog range down to a precise 10-bit digital value (0 - 1023). The firmware processes this analog feedback to extract the local ambient room temperature.
3. **RTC Match & Comparison Pipeline:** The hardware real-time clock continuously handles core counter registers. The scheduler sequentially matches the live contents of `HOUR` and `MIN` registers against an internal array configuration storing 10 custom system schedules.
4. **Conditional Display Driving:**
   - **Active State:** On a successful match condition where the specific array structure is active, a dedicated active message state is flagged. The text string scrolls across the LCD, and the **Green LED** is asserted.
   - **Idle State:** When the scheduled match is void, the system falls back to an idle screen showing continuous `HOUR:MIN:SEC` variables and the active numeric room temperature, alongside an asserted **Red LED**.
5. **Secure Administrative Mode:** Upon monitoring a mechanical logic toggle on the Admin Switch, the system halts standard tracking to spin into a state machine driven by a 4x4 keypad. The user can securely index variables, dynamically adjust live RTC registers (`sethour`, `setminute`), and globally flag message activation values.

---

## 🧩 Firmware Architecture & Modular Breakdown

The system code follows a strictly modular layout, segregating low-level hardware register access from high-level scheduling algorithms:

### 📡 Analog-to-Digital Converter (`adc.c` / `adc.h`)
Manages the hardware initialization and successive readout operations of the internal 10-bit converter block. It manages standard operational bit configurations:
- `PDN_BIT (Bit 21)`: Powers on the internal ADC block.
- `CLKDIV`: Scales PCLK down to a valid sampling clock frequency (ADCLK = 3 MHz).
- `START_CONV`: Operates bit manipulation routines to signal immediate conversions, reading completion status cleanly via checking the `DONE_BIT (Bit 31)`.

### Administrative State Engine (`admin.c` / `admin.h`)
Controls the user interface menus. It acts as an interactive state machine that parses inputs captured from raw matrix scans and channels them into targeted configuration functions:
- `rtc_edit()`: Dynamically processes numeric input validation bounds to write secure adjustments to time units.
- `event_edit()`: Allows the user to select specific indices within memory structures and toggle bit configurations to selectively display or skip alerts.

### Keypad Matrix Driver (`kpm.c` / `kpm.h`)
Implements an active grid scanning mechanism across dedicated I/O ports. Row pins (`P1.16 - P1.19`) are managed sequentially, checking column input states via bit masking macros (`READNIBBLE`) to provide precise debounced key recognition. It contains robust data aggregation handlers such as `ReadNumber()` and `Read4Number()` to filter invalid inputs against physical ceilings before passing them downstream.

### Low-Level Delay Pipeline (`delay.c` / `delay.h`)
Calculates raw block timing using hardcoded assembly-equivalent software execution loops optimized for core system speed frequencies, enabling deterministic tracking configurations across microsecond, millisecond, and standard second intervals.

---

## 📁 System Project Tree

```text
EventBoard-LPC2148-Firmware/
│
├── main.c              # Core execution cycle, initialization, and main system loop
├── admin.c             # Admin mode state machine logic (RTC & Event scheduling)
├── event.c             # Core schedule logic, array allocation, and text processing
├── adc.c               # LPC2148 On-chip 10-bit ADC bare-metal driver (LM35 reading)
├── rtc.c               # LPC2148 On-chip Hardware RTC read/write register controller
├── kpm.c               # 4x4 Matrix Keypad scanning driver with input filtering
├── lcd.c               # 16x2 Character LCD command and data driving module
├── delay.c             # Microcontroller clock-tuned delay loop calculations
│
├── admin.h             # Administrative engine function declarations
├── event.h             # Event message tracking configurations and definitions
├── adc.h               # ADC initialization and conversion prototypes
├── rtc.h               # RTC register manipulation mapping prototypes
├── kpm.h               # Keypad parsing mechanics and numerical entry prototypes
├── lcd.h               # LCD display function prototypes
├── delay.h             # Timing tracking function prototypes
│
├── adc_defines.h       # LPC2148 ADC register bitmasks and clock division calculations
├── kpmdefines.h        # Keypad matrix pin allocations and row/column bit definitions
├── lcddefines.h        # LCD instructions, commands, and configuration constraints
├── defines.h           # Low-level bit manipulation macros (SETBIT, CLRBIT, WRITENIBBLE)
├── types.h             # Clean, explicit standard data-type definitions (u8, u32, f32)
│
└── README.md           # System documentation
```
## Targeted Application Areas
1. Automated Educational Timetable Alerting: Acts as an automated notice hub inside academic institutions to broadcast lecture changes, classroom locations, lab schedules, or exam durations directly to students    without human intervention.
2. Industrial Shift and Maintenance Scheduling: Perfect for manufacturing spaces to automate visual alerts for structural shift rotations, automated breakdown warnings, or scheduled tooling checkups based on factory floor time.
3. Corporate Smart Offices: Replaces traditional stationary notice boards in corporate facilities to stream real-time updates regarding general assembly calls, boardroom reservations, or break reminders.
4. Healthcare & Clinical Alerts: Deployable in healthcare clinics for streaming patient care timetables, medication dispensing timelines, or automated safety check markers while checking ambient ward conditions.
## Future Architecture Enhancements
1. Non-Volatile Storage Integration: Add external $I2C-driven EEPROM support to store modifications to event attributes, preventing schedule loss during a power collapse.
2. Wireless Networking Capability: Interface an external UART Wi-Fi/Bluetooth stack to allow automated cloud scheduling updates, bypassing the physical keypad.
3. Dynamic DMA Scrolling: Transition display logic toward an interrupt-driven Direct Memory Access (DMA) or timer-based refresh layout to eliminate raw blocking delays.
## Project Outcomes:
1. Register-Level Competency: Manually engineered pure, hardware-level abstraction layers and drivers without relying on standard vendor runtime packages or third-party wrappers, confirming a strong mastery of low-level ARM7 architectures.
2. Defensive Firmware Execution: Built strict boundary filters inside data input channels to parse and discard anomalous user inputs before pushing modifications to actual hardware target registers.
3. Advanced Peripheral Cohabitation: Successfully optimized system resources to handle multi-bit character streams, continuous analog conversions, physical grid matrices, and timekeeping registers inside one unified firmware lifecycle.
4. Modular Code Reusability: Established a clean, decoupled file directory infrastructure that maintains a clear partition between high-level logic states and raw bare-metal hardware actions.
## Conclusion
**The EventBoard: RTC-Driven Message Display System **demonstrates a robust execution of bare-metal firmware design for high-reliability embedded platforms. By successfully structuring a clean hardware interaction framework between the internal real-time clock, multi-bit LCD logic, continuous analog metrics, and input matrices, the application maintains clock accuracy while staying completely responsive to local administrative modifications.  The resulting system forms an efficient blueprint for any automated tracking application, showing strong fundamentals in register manipulation, deterministic logic loops, and structured hardware-software codesign


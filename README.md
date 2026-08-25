# EventBoard – RTC-Driven Message Display System

An embedded digital notice board built using the **LPC2148 ARM7TDMI** microcontroller. It displays scheduled messages based on real-time clock conditions and shows ambient temperature using an **LM35 sensor**.

## Features

* RTC-based scheduled message display
* 16x2 LCD for messages, time, and temperature
* LM35 temperature monitoring using on-chip ADC
* 4x4 keypad for Admin Mode
* Real-time modification of scheduled events
* LED and buzzer status indication
* Register-level Embedded C programming

## Hardware

* **Microcontroller:** NXP LPC2148 ARM7
* **Display:** 16x2 LCD
* **Temperature Sensor:** LM35
* **Input:** 4x4 Matrix Keypad
* **Timekeeping:** On-chip RTC
* **ADC:** 10-bit on-chip ADC
* **Indicators:** LEDs and buzzer

## Software

* **Language:** Embedded C
* **IDE:** Keil uVision
* **Programming:** Flash Magic / UART0 ISP
* **Architecture:** Bare-metal, modular firmware

## Working

1. LPC2148 initializes the RTC, ADC, LCD, keypad, and GPIO.
2. RTC continuously checks the current time against stored schedules.
3. When a schedule matches, the corresponding message scrolls on the LCD.
4. During idle time, the LCD displays the current time and temperature.
5. Admin Mode allows authorized users to modify RTC settings and scheduled events.

## Key Learning

* LPC2148 ARM7 microcontroller programming
* Register-level peripheral interfacing
* ADC, RTC, GPIO, LCD, and keypad drivers
* Embedded C and modular firmware design
* Real-time event scheduling

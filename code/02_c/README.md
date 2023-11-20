Following examples are written in C and is intended for STM32 microcontrollers using the CMSIS (Cortex Microcontroller Software Interface Standard).

# [clockSetMulti.c](/code/02_c/clockSetMulti.c): STM32 Clock Configuration Example

The provided code demonstrates how to configure the clock system on an STM32 microcontroller dynamically. 
Ideally, these configurations are commonly done in a system initialization file named `system_stm32xxxx.c` (where "xxxx" represents the specific STM32 series, e.g., f10x, f4xx, etc.). 
For demonstration purposes, the clock configuration is included in the `main.c` file.

It includes functions to initialize the clock using the High-Speed Internal (HSI), High-Speed External (HSE), and Phase-Locked Loop (PLL) clock sources.
Additionally, it's recommended to handle interrupts for better efficiency, as busy delays in the program may affect the responsiveness of the microcontroller.

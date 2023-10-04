/* 
* File: test_board.c
* Project: LED Blinking on Port A to E on EasyMx PRO board.
* Description: This file is to test ports on the board.
*/

#include "stm32f10x.h"

/* Function prototypes */
void initClockHSI(void);
void initGPIO(void);
void delay(uint32_t time);


#define some_delay 500000

int main() {
    initClockHSI(); // Set system clock to HSI
    initGPIO();

    while (1) {
        // Iterate through ports A to E
        for (uint8_t port = 0; port < 5; port++) {
            GPIO_TypeDef *currentPort = (GPIO_TypeDef *)(GPIOA_BASE + (port * 0x400)); // Calculate the base address of the current port

            // Toggle all pins on the current port to turn the LEDs on/off
            currentPort->ODR ^= 0xFFFF;

            // Delay for a while
            delay(some_delay);
        }
    }

    return 0;
}

void initClockHSI(void) {
    RCC->CR |= RCC_CR_HSION; // Enable internal HSI(RC)

    while ((RCC->CR & RCC_CR_HSIRDY) != RCC_CR_HSIRDY); // Wait for HSI to be ready

    RCC->CFGR &= ~(RCC_CFGR_SW); // Clear SW bits
    RCC->CFGR |= RCC_CFGR_SW_HSI; // Set HSI as system clock

    while ((RCC->CFGR & RCC_CFGR_SWS_HSI) != RCC_CFGR_SWS_HSI); // Wait for HSI to be the system clock
}

void initGPIO(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPDEN |
                    RCC_APB2ENR_IOPEEN ; // Enable GPIO clocks for all ports (A to E)

    // Configure pins on all ports as outputs
    GPIOA->CRL = 0x33333333;
    GPIOA->CRH = 0x33333333;

    GPIOB->CRL = 0x33333333;
    GPIOB->CRH = 0x33333333;

    GPIOC->CRL = 0x33333333;
    GPIOC->CRH = 0x33333333;

    GPIOD->CRL = 0x33333333;
    GPIOD->CRH = 0x33333333;

    GPIOE->CRL = 0x33333333;
    GPIOE->CRH = 0x33333333;

   
}

void delay(uint32_t time) {
    while (time--);
}

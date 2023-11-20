/**
 * @file main.c
 * @brief STM32 Clock Configuration Example
 * @details The provided code demonstrates how to configure the clock system on an STM32 microcontroller dynamically.
 * Ideally, these configurations are commonly done in a system initialization file named system_stm32xxxx.c 
 * (where "xxxx" represents the specific STM32 series, e.g., f10x, f4xx, etc.). For demonstration purposes, 
 * the clock configuration is included in the main.c file.
 *
 * @note It includes functions to initialize the clock using the High-Speed Internal (HSI), High-Speed External (HSE), 
 * and Phase-Locked Loop (PLL) clock sources. Additionally, it's recommended to handle interrupts for better efficiency, 
 * as busy delays in the program may affect the responsiveness of the microcontroller.
 */
 
 #include "stm32f10x.h"

/**
 * @brief Define constants
 */
 
 
#define DELAY_TIME 499999   /**< Delay time for toggle */
#define DEBOUNCE_DELAY 100  /**< Delay for switch debouncing */


/**
 * @brief Function prototypes
 */
void initClockPLL(void);
void initClockHSI(void);
void initClockHSE(void);
void delay(volatile uint32_t time);

/**
 * @brief Main function
 * @details Initializes the clock and switches the clock source dynamically based on user input.
 * @return 0 on successful execution
 */
int main() {
    // Initialize the clock to use HSI as the system clock
    initClockHSI();

    RCC->APB2ENR |= RCC_APB2ENR_IOPDEN; // Enable port D clock

    // Configure Port D pins 0,5,6,7 as outputs
    GPIOD->CRL |= (GPIO_CRL_MODE0  | GPIO_CRL_MODE5 | GPIO_CRL_MODE6 | GPIO_CRL_MODE7);
    GPIOD->CRL &= ~(GPIO_CRL_CNF0  | GPIO_CRL_CNF5 | GPIO_CRL_CNF6 | GPIO_CRL_CNF7);

    GPIOD->ODR = 0; // Clear port
    GPIOD->ODR = (1 << 7); // Set PD7 to indicate HSI as active

    while (1) {
        GPIOD->ODR ^= (1 << 0); // Toggle pins 0
        delay(DELAY_TIME); // Introduce a delay

        // Check the state of pin 4 of Port D
        if ((GPIOD->IDR & GPIO_IDR_IDR4) == GPIO_IDR_IDR4) {
            delay(DEBOUNCE_DELAY); // Small delay for debouncing

            // Depending on the current system clock source, switch to a different source
            if ((RCC->CFGR & RCC_CFGR_SWS) == RCC_CFGR_SWS_HSI) {
                initClockHSE();
                GPIOD->ODR = (1 << 5); // Set PD5 to indicate HSE as active
            } else if ((RCC->CFGR & RCC_CFGR_SWS) == RCC_CFGR_SWS_HSE) {
                initClockPLL();
                GPIOD->ODR = (1 << 6); // Set PD6 to indicate PLL as active
            } else if ((RCC->CFGR & RCC_CFGR_SWS) == RCC_CFGR_SWS_PLL) {
                initClockHSI();
                GPIOD->ODR = (1 << 7); // Set PD7 to indicate HSI as active
            }
        }
    }

    return 0;
}

/**
 * @brief Function to introduce a delay
 * @param time Number of iterations for the delay
 */
void delay(volatile uint32_t time) {
    while (time--);
}

/**
 * @brief Initializes the clock to use 8MHz HSI as the system clock.
 */
void initClockHSI(void) {
    RCC->CR |= RCC_CR_HSION; // Enable internal HSI (RC)

    while ((RCC->CR & RCC_CR_HSIRDY) != RCC_CR_HSIRDY); // Wait for HSI to be ready

    RCC->CFGR &= (~RCC_CFGR_SW); // Clear SW bits
    RCC->CFGR |= RCC_CFGR_SW_HSI; // Set HSI as the system clock

    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI); // Wait for HSI to be the system clock
}

/**
 * @brief Initializes the clock to use an external clock source (HSE) at 25MHz.
 */
void initClockHSE(void) {
    RCC->CR |= RCC_CR_HSEON; // Enable HSE

    while ((RCC->CR & RCC_CR_HSERDY) != RCC_CR_HSERDY); // Wait for HSE to be ready

    RCC->CFGR &= (~RCC_CFGR_SW); // Clear SW bits
    RCC->CFGR |= RCC_CFGR_SW_HSE; // Set HSE as the system clock

    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSE); // Wait for HSE to be the system clock
}

/**
 * @brief Initializes the clock to use the PLL clock configuration.
 */
void initClockPLL(void) {
    // Check if PLL is used as the system clock
    if ((RCC->CFGR & RCC_CFGR_SWS) == RCC_CFGR_SWS_PLL) {
        // Set HSI as the system clock
        RCC->CFGR &= (~RCC_CFGR_SW);
        RCC->CFGR |= RCC_CFGR_SW_HSI;

        // Wait for HSI to be the system clock
        while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI) {
            // Implement a timeout mechanism for robustness
        }
    }

    // Disable the PLL
    RCC->CR &= (~RCC_CR_PLLON);

    // Wait until PLLRDY is cleared
    while ((RCC->CR & RCC_CR_PLLRDY) != 0) {
        // Implement a timeout mechanism for robustness
    }

    // Configure the PLL source as HSI/2
    RCC->CFGR &= (~RCC_CFGR_PLLSRC);
    RCC->CFGR |= RCC_CFGR_PLLSRC_PREDIV1;

    // Set the PLL multiplier to a desired value (e.g., 2)
    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_PLLMULL) | RCC_CFGR_PLLMULL_0;

    // Enable the PLL
    RCC->CR |= RCC_CR_PLLON;

    // Wait until PLLRDY is set
    while ((RCC->CR & RCC_CR_PLLRDY) == 0) {
        // Implement a timeout mechanism for robustness
    }

    // Set PLL as the system clock
    RCC->CFGR &= (~RCC_CFGR_SW);
    RCC->CFGR |= RCC_CFGR_SW_PLL;

    // Wait for the PLL to be the system clock
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) {
        // Implement a timeout mechanism for robustness
    }
}

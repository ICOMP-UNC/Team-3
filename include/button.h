/**
 * @file button.h
 * @brief Button control and debounce handling for STM32.
 *
 * This file provides functions and definitions for initializing buttons on GPIO ports,
 * reading button states, and managing debounce for reliable button presses.
 * It includes configurations for a STOP button and a CONTROL button, each connected to
 * specific pins on GPIOB. The debounce function helps to eliminate noise from mechanical
 * button presses, ensuring reliable detection of button states.
 */

#include "libopencm3/stm32/exti.h"
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/cm3/nvic.h"

/**
 * @brief GPIO port for the STOP button.
 */
#define STOP_BUTTON_PORT GPIOB

/**
 * @brief GPIO pin for the STOP button.
 */
#define STOP_BUTTON_PIN GPIO10

/**
 * @brief GPIO port for the CONTROL button.
 */
#define CONTROL_BUTTON_PORT GPIOB

/**
 * @brief GPIO pin for the CONTROL button.
 */
#define CONTROL_BUTTON_PIN GPIO11

/**
 * @brief Debounce delay in milliseconds.
 *
 * Defines the delay duration for debounce logic to filter out
 * noise from mechanical button presses.
 */
#define DEBOUNCE_DELAY 200  // Delay for debounce in [ms]

/**
 * @brief Initializes the buttons on specific GPIO pins with EXTI.
 *
 * This function sets up the GPIO pins for the STOP and CONTROL buttons as
 * inputs with pull-up resistors and enables external interrupt requests on
 * both pins with specified edge triggers.
 */
void button_init(void);

/**
 * @brief Retrieves the current stop flag state.
 *
 * This function returns the status of the stop flag, which indicates whether
 * the STOP button has been pressed.
 *
 * @return The state of the stop flag (0 or 1).
 */
uint8_t button_get_stop_flag(void);

/**
 * @brief Retrieves the current object flag state.
 *
 * This function returns the status of the object flag, which indicates whether
 * the CONTROL button has triggered an object detection event.
 *
 * @return The state of the object flag (0 or 1).
 */
uint8_t button_get_object_flag(void);

/**
 * @brief Sets the state of the object flag.
 *
 * This function allows setting the object flag to a specified boolean value,
 * typically used to reset the flag after handling an object detection event.
 *
 * @param boolean The new state to set for the object flag (0 or 1).
 */
void button_set_object_flag(uint8_t boolean);

/**
 * @brief Debounce function to filter out noise from button presses.
 *
 * This function checks if the required debounce delay has passed since the last valid button press.
 * If the time difference between the current time and the last recorded time exceeds `DEBOUNCE_DELAY`,
 * the function updates the last recorded time and returns true, allowing the interrupt to proceed.
 *
 * @param current_time The current time from the SysTick timer.
 * @param last_time Pointer to the last recorded time for the interrupt line.
 * @return 1 if the debounce delay has passed and the interrupt is valid, 0 otherwise.
 */
static inline int debounce(uint32_t current_time, uint32_t *last_time);

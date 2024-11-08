/**
 * @file motor_control.h
 * @brief Motor control driver using PWM for STM32 with libopencm3.
 *
 * This file contains the functions needed to initialize and control the power output
 * of a motor using PWM on a specific GPIO pin.
 */

#include "libopencm3/stm32/gpio.h"
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/timer.h"

#define MOTOR_PIN       GPIO0 /**< GPIO pin for the motor control output */
#define MOTOR_PERIOD_MS 50    /**< PWM period in milliseconds */

/**
 * @brief Initializes the motor control GPIO and timer for PWM output.
 *
 * This function sets up the GPIO pin and timer necessary to generate a PWM signal
 * for motor control. The timer is configured for the appropriate period and output mode.
 */
void motor_init(void);

/**
 * @brief Sets the motor power by adjusting the PWM duty cycle.
 *
 * This function adjusts the PWM duty cycle to control the motor power output.
 *
 * @param percentage Power level as a percentage (0 to 100).
 */
void motor_set_power(uint8_t percentage);

/**
 * @brief Disables the motor output by turning off the PWM signal.
 *
 * This function stops the motor by disabling the PWM output on the configured pin.
 */
void motor_disable(void);

/**
 * @brief Enables the motor output by turning on the PWM signal.
 *
 * This function starts the motor by enabling the PWM output on the configured pin.
 */
void motor_enable(void);

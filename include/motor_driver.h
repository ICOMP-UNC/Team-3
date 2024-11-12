/**
 * @file motor_driver.h
 * @brief Motor control driver using PWM for STM32 with libopencm3.
 *
 * This file contains the functions needed to initialize and control the power output
 * of a motor using PWM on a specific GPIO pin. The driver allows setting up PWM,
 * adjusting power levels by modifying the duty cycle, and enabling/disabling motor control.
 */

#include "libopencm3/stm32/gpio.h"
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/timer.h"

/** GPIO pin for the motor control output */
#define MOTOR_PIN GPIO0

/** GPIO port for the motor control output */
#define MOTOR_PORT GPIOB

/** PWM period in milliseconds for motor control */
#define MOTOR_PERIOD_MS 50

/** Motor state when disabled */
#define MOTOR_DISABLED 0

/** Motor state when enabled */
#define MOTOR_ENABLED 1

/** Timer used to generate PWM for motor control */
#define MOTOR_TIMER TIM3

/**
 * @brief Initializes the motor control GPIO and timer for PWM output.
 *
 * This function sets up the GPIO pin and timer necessary to generate a PWM signal
 * for motor control. The timer is configured with the appropriate period and
 * output mode to ensure smooth and precise control of the motor speed.
 */
void motor_init(void);

/**
 * @brief Sets the motor power by adjusting the PWM duty cycle.
 *
 * This function adjusts the PWM duty cycle to control the motor power output.
 * The power is set as a percentage, where 0% turns the motor off and 100%
 * sets it to maximum power.
 *
 * @param percentage Power level as a percentage (0 to 100).
 */
void motor_set_power(uint8_t percentage);

/**
 * @brief Disables the motor output by turning off the PWM signal.
 *
 * This function stops the motor by disabling the PWM output on the configured pin,
 * setting the motor to its `MOTOR_DISABLED` state.
 */
void motor_disable(void);

/**
 * @brief Enables the motor output by turning on the PWM signal.
 *
 * This function starts the motor by enabling the PWM output on the configured pin,
 * setting the motor to its `MOTOR_ENABLED` state.
 */
void motor_enable(void);

uint8_t motor_get_state(void);

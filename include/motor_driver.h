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

/**
 * @brief GPIO pin used for motor control.
 *
 * This pin outputs the PWM signal that controls the motor’s speed and state.
 */
#define MOTOR_PIN GPIO0

/**
 * @brief GPIO port for motor control.
 *
 * Defines the port associated with the motor control pin.
 */
#define MOTOR_PORT GPIOB

/**
 * @brief PWM period for motor control, specified in milliseconds.
 *
 * Determines the frequency of the PWM signal, controlling the speed granularity.
 */
#define MOTOR_PERIOD_MS 50

/**
 * @brief Motor disabled state value.
 *
 * A constant representing the motor's disabled state.
 */
#define MOTOR_DISABLED 0

/**
 * @brief Motor enabled state value.
 *
 * A constant representing the motor's enabled state.
 */
#define MOTOR_ENABLED 1

/**
 * @brief Timer used for generating PWM to control the motor.
 *
 * Specifies the hardware timer associated with PWM generation.
 */
#define MOTOR_TIMER TIM3

/**
 * @brief Conversion factor from milliseconds to seconds.
 *
 * Used to convert time periods in milliseconds to seconds.
 */
#define MS_TO_SEC 1000

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

/**
 * @brief Retrieves the current state of the motor.
 *
 * Returns the motor state, indicating whether it is enabled (`MOTOR_ENABLED`)
 * or disabled (`MOTOR_DISABLED`).
 *
 * @return The current motor state (0 for disabled, 1 for enabled).
 */
uint8_t motor_get_state(void);

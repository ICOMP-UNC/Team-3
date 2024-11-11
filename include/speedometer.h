/**
 * @file speedometer.h
 * @brief Speedometer module for measuring rotational speed in RPM and rad/s.
 *
 * This module provides functions to initialize a speedometer using an encoder
 * connected to TIM2 as an external clock, and functions to retrieve the RPM and
 * radian-per-second (rad/s) values. It configures the required timers and DMA
 * to calculate rotational speed based on encoder pulses.
 */

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/dma.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>

/** High pulse count for one full rotation, defines encoder resolution. */
#define PULSES_1TURN TIM_IC_PSC_8

/** GPIO pin connected to the speedometer sensor. */
#define SPEEDOMETER_PIN GPIO0

/** GPIO port connected to the speedometer sensor. */
#define SPEEDOMETER_PORT GPIOA

/** Timer used to measure intervals in milliseconds for speed calculation. */
#define TENMS_TIMER TIM1

/** Timer configured as an encoder timer for measuring rotational speed. */
#define ENCODER_TIMER TIM2

/** DMA channel used for transferring data in speedometer operations. */
#define DMA_CH DMA_CHANNEL2

/** Prescaler value for generating a 600 ms interval, used with TENMS_TIMER. */
#define MS_PS 719

/** Timer update interval for measuring rotation, set to 600 ms. */
#define MS_INTERVAL 59999

/** Constant to convert encoder counts to radian per second (rad/s). */
#define CONSTANT_TO_RAD_S 1.309

/** Constant to convert encoder counts to revolutions per minute (RPM). */
#define CONSTANT_TO_RPM 12.5

/**
 * @brief Initializes the speedometer module.
 *
 * Configures the necessary peripherals, including the encoder timer (TIM2),
 * the control timer (TIM1), and the DMA channel used for data transfer.
 * Sets up the speedometer GPIO as an input and enables external clocking
 * on TIM2 to measure rotational speed based on encoder pulses.
 */
void speedometer_init(void);

/**
 * @brief Gets the current speed in revolutions per minute (RPM).
 *
 * Reads the pulse count from the encoder and calculates the speed in RPM
 * based on the predefined constants. This function converts pulse data
 * into a standard RPM measurement.
 *
 * @return The current speed in RPM.
 */
float speedometer_getRPM(void);

/**
 * @brief Gets the current speed in radians per second (rad/s).
 *
 * Uses the pulse count from the encoder to calculate speed in rad/s.
 * This function provides a measure of rotational speed in radians
 * per second, useful for precise control calculations.
 *
 * @return The current speed in radians per second (rad/s).
 */
float speedometer_getRAD_S(void);


uint16_t abs(int16_t num);
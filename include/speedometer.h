/**
 * @file speedometer.h
 * @brief Speedometer module for measuring rotational speed in RPM and rad/s.
 *
 * This module provides functions to initialize a speedometer using an encoder
 * connected to TIM2 as an external clock, and functions to retrieve the RPM and
 * radian-per-second (rad/s) values.
 */

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/dma.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>

/** @brief High pulse count for one full rotation. */
#define PULSES_1TURN TIM_IC_PSC_8

/** @brief GPIO pin connected to the speedometer sensor. */
#define SPEEDOMETER_PIN GPIO0

/** @brief GPIO port connected to the speedometer sensor. */
#define SPEEDOMETER_PORT GPIOA

/** @brief Timer used to measure intervals in milliseconds. */
#define TENMS_TIMER TIM1

/** @brief Timer configured as an encoder timer for speed measurement. */
#define ENCODER_TIMER TIM2

/** @brief DMA channel used for transferring data in speedometer operations. */
#define DMA_CH DMA_CHANNEL2

/** @brief Prescaler value for generating a 600 ms interval. */
#define MS_PS 719

/** @brief Timer update interval of 600 ms. */
#define MS_INTERVAL 59999

/** @brief Constant to convert to radian per second (rad/s). */
#define CONSTANT_TO_RAD_S 1.309

/** @brief Constant to convert to revolutions per minute (RPM). */
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
 * based on the predefined constants.
 *
 * @return The current speed in RPM.
 */
float speedometer_getRPM(void);

/**
 * @brief Gets the current speed in radians per second (rad/s).
 *
 * Uses the pulse count from the encoder to calculate speed in rad/s.
 *
 * @return The current speed in radians per second (rad/s).
 */
float speedometer_getRAD_S(void);

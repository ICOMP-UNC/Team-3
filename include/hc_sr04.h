/**
 * @file hcsr04.h
 * @brief Ultrasonic sensor HC-SR04 driver for STM32 using libopencm3.
 *
 * This file contains the functions necessary to initialize and retrieve
 * the distance measured by the HC-SR04 ultrasonic sensor.
 */

#include "libopencm3/cm3/nvic.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/timer.h"

#define TRIG_PIN GPIO8 /**< Trigger pin for the HC-SR04 */
#define ECHO_PIN GPIO9 /**< Echo pin for the HC-SR04 */

#define HCSR04_PORT  GPIOB
#define HCSR04_TIMER TIM4

/**
 * @brief Initializes the pins and timers needed for the HC-SR04.
 *
 * This function configures the trigger pin (TRIG_PIN) as an output and the echo
 * pin (ECHO_PIN) as an input, setting up the necessary timers to measure the
 * duration of the echo pulse.
 */
void hcsr04_init(void);

/**
 * @brief Sends a 10 µs trigger pulse to the HC-SR04 sensor.
 *
 * This function activates the trigger pin (TRIG_PIN) for 10 microseconds,
 * generating a pulse to start the distance measurement on the HC-SR04.
 */
void hcsr04_trigger(void);

/**
 * @brief Retrieves the distance measured by the HC-SR04 sensor in centimeters.
 *
 * This function measures the time taken for the echo pulse to return to the
 * echo pin (ECHO_PIN) and calculates the distance in centimeters.
 *
 * @return The measured distance in centimeters.
 */
float hcsr04_get_distance(void);

/**
 * @brief Applies saturation to the distance values to avoid out-of-range results.
 *
 * This function ensures that the distance values remain within a reasonable range,
 * discarding any outlier or extreme values.
 *
 * @return The corrected distance value after applying saturation.
 */
float Saturation(void);

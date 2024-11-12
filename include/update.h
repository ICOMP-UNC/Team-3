/**
 * @file update.h
 * @brief System update functions for speed and distance measurement.
 *
 * This file contains functions for initializing the SysTick timer, handling
 * periodic system updates (such as PID updates, LCD display updates, and distance measurements),
 * and calculating the average distance measurement.
 */

#include "button.h"
#include "hc_sr04.h"
#include "lcd.h"
#include "motor_driver.h"
#include "pid.h"
#include "setpoint.h"
#include "speedometer.h"
#include "utils.h"
#include <libopencm3/cm3/systick.h>

/**
 * @brief Number of ticks for a 1 ms interval at a system clock of 72 MHz.
 */
#define TICKS_FOR_MS 72000

/** @brief Sample time interval in milliseconds for PID updates. */
#define PID_RATE 50

/** @brief Sample time interval in milliseconds for object distance measurements. */
#define MEASUREMENT_RATE 70

/** @brief Number of samples taken for each distance measurement average. */
#define N_MEASUREMENT 10

/** @brief Threshold distance for measurement evaluation in [cm] */
#define MEASUREMENT_TRHS 10

/** @brief Sample time interval in milliseconds for updating the display. */
#define DISPLAY_RATE 200

/**
 * @brief Maximum revolutions per minute (RPM) for the motor.
 *
 * This constant defines the upper limit for the motor speed in RPM. It is used
 * as a reference for setting the motor's speed range and calculating the setpoint.
 */
#define MAX_RPM 6500

/**
 * @brief Initializes the SysTick timer for system timing.
 *
 * Sets the reload value for generating an interrupt every millisecond, selects
 * the AHB clock as the source, and enables both the counter and the interrupt.
 */
void update_init(void);

/**
 * @brief Retrieves the current system time in milliseconds.
 *
 * This function returns the current SysTick counter value, which can be used
 * to track elapsed time within the system.
 *
 * @return Current system time in milliseconds.
 */
uint32_t get_system_time_ms(void);

/**
 * @brief Calculates the average of recorded measurements.
 *
 * This function iterates through the measurement buffer and calculates the
 * average distance. Used primarily for processing distance measurements from
 * the sensor.
 *
 * @return The average distance measurement.
 */
float get_measurement_prom(void);

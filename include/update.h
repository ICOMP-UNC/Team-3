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

/** @brief Sample time interval in milliseconds for updating the display. */
#define DISPLAY_RATE 500

/** @brief Sample time interval in milliseconds for object distance measurements. */
#define MEASUREMENT_RATE 120

/** @brief Number of samples taken for each distance measurement average. */
#define N_MEASUREMENT 10

/** @brief Threshold distance for measurement evaluation in [cm]. */
#define MEASUREMENT_TRHS 10

/** @brief Duration in milliseconds to display the measurement result. */
#define MEASUREMENT_DISPLAY_TIME 3000

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
 * @brief Calculates the average of recorded measurements.
 *
 * This function iterates through the measurement buffer and calculates the
 * average distance. Used primarily for processing distance measurements from
 * the sensor.
 *
 * @return The average distance measurement.
 */
float get_measurement_prom(void);

/**
 * @brief Displays the current speed and target setpoint on the LCD.
 *
 * Clears the LCD screen and shows the RPM of the motor along with the desired
 * setpoint, updating the display with the latest values.
 */
void display_speed(void);

/**
 * @brief Records distance measurements from the ultrasonic sensor.
 *
 * This function performs distance measurements using the ultrasonic sensor,
 * stores the results in a buffer, and determines if the measurement meets
 * the pass/fail threshold.
 */
void measure(void);

/**
 * @brief Displays measurement information on the LCD.
 *
 * Depending on whether a full set of measurements has been completed, this
 * function will either display the measurement percentage or the average
 * distance value. Used to provide feedback on object height or measurement
 * completion.
 */
void display_measure_info(void);

/**
 * @brief Updates the PID controller for motor speed control.
 *
 * Retrieves the current potentiometer value for the setpoint, obtains the
 * current RPM from the speedometer, and adjusts motor power output based on
 * the PID controller's calculations.
 */
void upt_pid(void);

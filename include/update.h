#include <libopencm3/cm3/systick.h>
#include "hc_sr04.h"
#include "lcd.h"
#include "motor_driver.h"
#include "pid.h"
#include "setpoint.h"
#include "speedometer.h"
#include "utils.h"

/**
 * @brief Number of ticks for a 50 ms interval at a system clock of 72 MHz.
 *
 * This value is calculated based on a system clock of 72 MHz. With each tick
 * representing 1/72,000,000 seconds, 3,600,000 ticks are required for a delay of 50 ms.
 */
#define TICKS_FOR_50MS 3600000

/**
 * @brief Maximum revolutions per minute (RPM) for the motor.
 *
 * This constant defines the upper limit for the motor speed in RPM. It is used
 * as a reference for setting the motor's speed range and calculating the setpoint.
 */
#define MAX_RPM 6500

/**
 * @brief Configures SysTick to trigger an interrupt every 50 ms.
 *
 * This function sets up the SysTick timer to use the system clock (AHB) as
 * its source and sets the reload value to generate an interrupt every 50 ms.
 * At a system clock of 72 MHz, this corresponds to a reload value of 3,600,000.
 */
void systick_init(void);

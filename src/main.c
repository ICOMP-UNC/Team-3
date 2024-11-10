#include "libopencm3/stm32/gpio.h"
#include "libopencm3/stm32/i2c.h"
#include "libopencm3/stm32/rcc.h"

#include "../include/hc_sr04.h"
#include "../include/lcd.h"
#include "../include/motor_driver.h"
#include "../include/pid.h"
#include "../include/speedometer.h"
#include "../include/utils.h"
#pragma once

#define TRUE  1
#define FALSE 0

void systemInit(void);

float filtered_speed = 0;
PID_Controller c;

int main(void)
{
    systemInit();
    lcd_init();
    // hcsr04_init();
    motor_init();
    speedometer_init();
    c.kd = 0.0018;
    c.ki = 0.0025;
    c.kp = 0.0015;
    c.setpoint = 4000;

    pid_init(&c);

    while (TRUE)
    {
        filtered_speed = speedometer_getRPM();
        motor_set_power(pid_update(filtered_speed));
        lcd_print_string("speed:");
        lcd_print_string(float_to_string(filtered_speed));
        delay_ms(100);

        lcd_clear();
    }
    return 0;
}

void systemInit(void)
{
    rcc_clock_setup_in_hse_8mhz_out_72mhz();
}

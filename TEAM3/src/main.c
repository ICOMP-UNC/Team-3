#include "libopencm3/stm32/gpio.h"
#include "libopencm3/stm32/i2c.h"
#include "libopencm3/stm32/rcc.h"

#include "../include/lcd.h"
#include "../include/hc_sr04.h"
#include "../include/motor_driver.h"
#include "../include/speedometer.h"
#include "../include/utils.h"
#pragma once

#define TRUE 1
#define FALSE 0

void systemInit(void);

uint8_t i = 40;

int main(void) {
    systemInit();
    lcd_init();
    hcsr04_init();
    motor_init();
    speedometer_init();

    while (TRUE)
    {
        lcd_print_string("speed:");
        lcd_print_string(float_to_string(speedometer_getRPM()));
        delay_ms(100);
        lcd_clear();
        //i = (i==100)? 0 : i+5;
        motor_set_power(i);
    }
    return 0;
}

void systemInit(void) {
    rcc_clock_setup_in_hse_8mhz_out_72mhz();
}


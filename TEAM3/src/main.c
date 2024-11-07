#include "libopencm3/stm32/gpio.h"
#include "libopencm3/stm32/i2c.h"
#include "libopencm3/stm32/rcc.h"

#include "../include/lcd.h"
#include "../include/hc_sr04.h"
#include "../include/utils.h"
#pragma once

#define TRUE 1
#define FALSE 0

void systemInit(void);

int main(void) {
    systemInit();
    lcd_init();
    hcsr04_init();

    while (TRUE)
    {
        lcd_print_string("Dist:");
        lcd_print_string(float_to_string(hcsr04_get_distance()));
        delay_ms(1000);
        lcd_clear();
    }
    return 0;
}

void systemInit(void) {
    rcc_clock_setup_in_hse_8mhz_out_72mhz();
}


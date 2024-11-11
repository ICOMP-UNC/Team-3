#include "hc_sr04.h"
#include "lcd.h"
#include "motor_driver.h"
#include "pid.h"
#include "setpoint.h"
#include "speedometer.h"
#include "utils.h"
#include "exti.h"

#define TRUE  1
#define FALSE 0

#define MAX_RPM 6500

void systemInit(void);

float filtered_speed = 0;
PID_Controller c;

int main(void)
{
    systemInit();
    exti_setup();
    lcd_init();
    hcsr04_init();
    pot_init();
    motor_init();
    speedometer_init();

    c.kd = 0.0006;
    c.ki = 0.00126;
    c.kp = 0.0013;
    c.setpoint = MAX_RPM;

    pid_init(&c);
    float set;
    while (TRUE)
    {
        set = pot_get_value() * MAX_RPM / 100;
        pid_setpoint(set);
        filtered_speed = speedometer_getRPM();
        motor_set_power(pid_update(filtered_speed));
        lcd_print_string("speed:");
        lcd_print_string(float_to_string(filtered_speed));
        lcd_set_cursor(2, 0);
        lcd_print_string("SP:");
        lcd_print_string(float_to_string(set));
        delay_ms(50); // TODO: Make update Rate with Systick

        lcd_clear();
    }
    return 0;
}

void systemInit(void)
{
    rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);
}

#include "update.h"

#define TRUE  1
#define FALSE 0

#define MAX_RPM 6500

void systemInit(void);

PID_Controller c;

int main(void)
{
    systemInit();
    speedometer_init();
    hcsr04_init();
    pot_init();
    lcd_init();
    motor_init();
    update_init();
    button_init();

    c.kd = 0.0006;
    c.ki = 0.00126;
    c.kp = 0.0013;
    c.setpoint = MAX_RPM;

    pid_init(&c);

    while (TRUE)
    {
        ;
    }
    return 0;
}

void systemInit(void)
{
    rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);
}

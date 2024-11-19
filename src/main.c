#include "update.h"

#define TRUE  1
#define FALSE 0

#define MAX_RPM              6500
#define INITIAL_DERIVATIVE   0.0006;
#define INITIAL_INTEGRAL     0.00126;
#define INITIAL_PROPORTIONAL 0.0013;

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

    c.kd = INITIAL_DERIVATIVE;
    c.ki = INITIAL_INTEGRAL;
    c.kp = INITIAL_PROPORTIONAL;
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

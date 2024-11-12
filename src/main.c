#include "update.h"

#define TRUE  1
#define FALSE 0

PID_Controller c;

/** Functions Prototypes */
void systemInit(void);

int main(void)
{
    systemInit();
    exti_setup();
    lcd_init();
    hcsr04_init();
    pot_init();
    motor_init();
    speedometer_init();
    systick_init();

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

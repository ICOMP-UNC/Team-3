#include "exti.h"
#include "hc_sr04.h"
#include "lcd.h"
#include "motor_driver.h"
#include "pid.h"
#include "setpoint.h"
#include "speedometer.h"
#include "update.h"
#include "utils.h"

#define TRUE  1
#define FALSE 0

#define MAX_RPM 6500

void systemInit(void);

static float speed = 0;
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
    update_init();

    c.kd = 0.0006;
    c.ki = 0.00126;
    c.kp = 0.0013;
    c.setpoint = MAX_RPM;

    pid_init(&c);
    float set;
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

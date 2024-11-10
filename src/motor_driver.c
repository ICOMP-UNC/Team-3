#include "../include/motor_driver.h"

static uint8_t motor_state = 0;

void motor_init()
{
    //Enables Clock
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_TIM3);

    gpio_set_mode(MOTOR_PORT, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, MOTOR_PIN);

    //TIMER Mode Counter ascending y auto Load
    timer_set_mode(MOTOR_TIMER, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    timer_enable_preload(MOTOR_TIMER);

    timer_set_prescaler(MOTOR_TIMER, 72-1);                  //Sets to 1 [MHZ] tick
    timer_set_period(MOTOR_TIMER, MOTOR_PERIOD_MS*1000 - 1); //Sets the signal Period

    //Configs Output Capture on pin
    timer_disable_oc_output(MOTOR_TIMER, TIM_OC3);
    timer_set_oc_mode(MOTOR_TIMER, TIM_OC3, TIM_OCM_PWM1);
    timer_set_oc_value(MOTOR_TIMER, TIM_OC3, (MOTOR_PERIOD_MS*1000)/100); //DC 1%
    motor_enable();

    timer_enable_counter(MOTOR_TIMER);
}

void motor_set_power(uint8_t percentage)
{
    if(!motor_state)
    {
        motor_enable();
        motor_state = 1;
    }
    if(percentage > 100) percentage = 100;
    timer_set_oc_value(MOTOR_TIMER, TIM_OC3, (MOTOR_PERIOD_MS*1000*percentage)/100);
}

void motor_disable()
{
    motor_state = 0;
    timer_disable_oc_output(MOTOR_TIMER, TIM_OC3);
}

void motor_enable()
{
    motor_state = 1;
    timer_enable_oc_output(MOTOR_TIMER, TIM_OC3);
}

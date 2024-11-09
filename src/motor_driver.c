#include "../include/motor_driver.h"

void motor_init()
{
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_TIM3);

    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, MOTOR_PIN);

    timer_set_mode(TIM3, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    timer_enable_preload(TIM3);

    timer_set_prescaler(TIM3, 72 - 1); // Coloca a 1Mhz
    timer_set_period(TIM3, MOTOR_PERIOD_MS * 1000 - 1);

    // Configura el canal 1 de TIM1 para PWM
    timer_disable_oc_output(TIM3, TIM_OC3);
    timer_set_oc_mode(TIM3, TIM_OC3, TIM_OCM_PWM1);
    timer_set_oc_value(TIM3, TIM_OC3, (MOTOR_PERIOD_MS * 1000) / 100); // DC 1%
    timer_enable_oc_output(TIM3, TIM_OC3);

    // Habilita la salida principal para el PWM
    timer_enable_break_main_output(TIM3);

    timer_enable_counter(TIM3);
}

void motor_set_power(uint8_t percentage)
{
    if (percentage > 100)
        percentage = 100;
    timer_set_oc_value(TIM3, TIM_OC3, (MOTOR_PERIOD_MS * 1000 * percentage) / 100);
}

void motor_disable()
{
    timer_disable_oc_output(TIM3, TIM_OC3);
}

void motor_enable()
{
    timer_enable_oc_output(TIM3, TIM_OC3);
}
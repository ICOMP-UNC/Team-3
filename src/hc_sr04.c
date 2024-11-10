#include "../include/HC_SR04.h"

volatile static uint32_t times[2];
volatile static float distance = 0; //[cm]
volatile static uint8_t conversion_flag = 0;

void hcsr04_init(void)
{
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_TIM4);

    gpio_set_mode(HCSR04_PORT, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, TRIG_PIN);
    gpio_clear(HCSR04_PORT, TRIG_PIN);

    gpio_set_mode(HCSR04_PORT, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, ECHO_PIN);

    // Config TIMER for Capture Input CH 4 (PB9) RISING
    timer_ic_set_input(HCSR04_TIMER, TIM_IC1, TIM_IC_IN_TI4);
    timer_ic_set_polarity(HCSR04_TIMER, TIM_IC1, TIM_IC_RISING);
    timer_ic_enable(HCSR04_TIMER, TIM_IC1);

    // Config TIMER for Capture Input CH 4 (PB9) FALLING
    timer_ic_set_input(HCSR04_TIMER, TIM_IC2, TIM_IC_IN_TI4);
    timer_ic_set_polarity(HCSR04_TIMER, TIM_IC2, TIM_IC_FALLING);
    timer_ic_enable(HCSR04_TIMER, TIM_IC2);

    timer_set_prescaler(HCSR04_TIMER, (rcc_apb1_frequency / 1000000) - 1); // Sets Preescaler to 1[uS] ticks
    timer_enable_counter(HCSR04_TIMER);

    timer_enable_irq(HCSR04_TIMER, TIM_DIER_CC1IE | TIM_DIER_CC2IE);
    nvic_enable_irq(NVIC_TIM4_IRQ);
}

void hcsr04_trigger(void)
{
    gpio_set(GPIOA, TRIG_PIN);
    for (volatile uint8_t i = 0; i < 30; i++);
    gpio_clear(GPIOA, TRIG_PIN);
}

void tim4_isr(void)
{
    if (timer_get_flag(HCSR04_TIMER, TIM_SR_CC1IF))
    {
        timer_clear_flag(HCSR04_TIMER, TIM_SR_CC1IF);
        times[0] = timer_get_counter(HCSR04_TIMER);
        conversion_flag = 0;
    }
    else
    {
        timer_clear_flag(HCSR04_TIMER, TIM_SR_CC2IF);
        times[1] = timer_get_counter(HCSR04_TIMER);
        distance = ((times[1] - times[0])) / 116;
        conversion_flag = 1;
    }
}

float hcsr04_get_distance(void)
{
    hcsr04_trigger();
    while (!conversion_flag);
    return Saturation();
}

float Saturation(void)
{
    if (distance > 400.0)
    {
        return 400.0;
    }
    else if (distance <= 2)
    {
        return 2.0;
    }
    else
    {
        return distance;
    }
}

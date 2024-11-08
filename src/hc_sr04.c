#include "../include/HC_SR04.h"

volatile static uint32_t times[2];
volatile static float distance = 0; //[cm]
volatile static uint8_t conversion_flag = 0;

void hcsr04_init(void)
{
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_TIM2);

    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, TRIG_PIN);
    gpio_clear(GPIOA, TRIG_PIN);

    gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, ECHO_PIN);

    // Configuro el timer 2 para captura en el canal 1 en Subida
    timer_ic_set_input(TIM2, TIM_IC1, TIM_IC_IN_TI1);
    timer_ic_set_polarity(TIM2, TIM_IC1, TIM_IC_RISING);
    timer_ic_enable(TIM2, TIM_IC1);

    // Configuro el timer 2 para captura en el canal 2 en Bajada
    timer_ic_set_input(TIM2, TIM_IC2, TIM_IC_IN_TI1);
    timer_ic_set_polarity(TIM2, TIM_IC2, TIM_IC_FALLING);
    timer_ic_enable(TIM2, TIM_IC2);

    timer_set_prescaler(TIM2, (rcc_apb1_frequency / 1000000) - 1); // Seteo el preescaler para 1uS
    timer_enable_counter(TIM2);

    timer_enable_irq(TIM2, TIM_DIER_CC1IE | TIM_DIER_CC2IE);
    nvic_enable_irq(NVIC_TIM2_IRQ);
}

void hcsr04_trigger(void)
{
    gpio_set(GPIOA, TRIG_PIN);
    for (volatile uint8_t i = 0; i < 30; i++)
        ;
    gpio_clear(GPIOA, TRIG_PIN);
}

void tim2_isr(void)
{
    if (timer_get_flag(TIM2, TIM_SR_CC1IF)) // Subida
    {
        timer_clear_flag(TIM2, TIM_SR_CC1IF);
        times[0] = timer_get_counter(TIM2);
        conversion_flag = 0;
    }
    else // Bajada
    {
        timer_clear_flag(TIM2, TIM_SR_CC2IF);
        times[1] = timer_get_counter(TIM2);
        distance = ((times[1] - times[0])) / 116;
        conversion_flag = 1;
    }
}

float hcsr04_get_distance(void)
{
    hcsr04_trigger();
    while (!conversion_flag)
        ;
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
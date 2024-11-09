#include "../include/speedometer.h"

volatile static uint16_t times[N_PULSE];

void speedometer_init(void)
{
    // Enable clocks for TIM4, DMA1, and GPIO
    rcc_periph_clock_enable(RCC_TIM4);
    rcc_periph_clock_enable(RCC_DMA1);
    rcc_periph_clock_enable(RCC_AFIO);
    rcc_periph_clock_enable(RCC_GPIOB);

    gpio_set_mode(SPEEDOMETER_PORT, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, SPEEDOMETER_PIN);

    // Configuro el timer 4 para captura en el canal 3 en Subida
    timer_ic_set_input(TIM4, TIM_IC3, TIM_IC_IN_TI3);
    timer_ic_set_polarity(TIM4, TIM_IC3, TIM_IC_RISING);
    timer_ic_set_filter(TIM4, TIM_IC3, TIM_IC_CK_INT_N_4); // Habilito el filtro
    timer_ic_enable(TIM4, TIM_IC3);

    // Set timer prescaler to 1ms (assuming 72 MHz APB1 clock)
    timer_set_prescaler(TIM4, (rcc_apb1_frequency / 1000) - 1);

    // Enable DMA request on TIM4 Channel 3
    timer_enable_irq(TIM4, TIM_DIER_CC3DE | TIM_DIER_UDE);

    // Configuro el DMA
    dma_channel_reset(DMA1, DMA_CHANNEL5);
    dma_set_priority(DMA1, DMA_CHANNEL5, DMA_CCR_PL_VERY_HIGH);
    dma_set_peripheral_address(DMA1, DMA_CHANNEL5, (uint32_t)&TIM4_CCR3);
    dma_set_memory_address(DMA1, DMA_CHANNEL5, (uint32_t)times);
    dma_set_number_of_data(DMA1, DMA_CHANNEL5, N_PULSE);
    dma_set_peripheral_size(DMA1, DMA_CHANNEL5, DMA_CCR_PSIZE_16BIT);
    dma_set_memory_size(DMA1, DMA_CHANNEL5, DMA_CCR_MSIZE_16BIT);
    dma_enable_circular_mode(DMA1, DMA_CHANNEL5);
    dma_enable_memory_increment_mode(DMA1, DMA_CHANNEL5);

    timer_enable_counter(TIM4);
    dma_enable_channel(DMA1, DMA_CHANNEL5);
}

float speedometer_getRPM(void)
{
    return (60 * 1000) / (abs(times[1] - times[0]) * 4);
}

uint16_t abs(uint16_t val)
{
    if (val < 0)
    {
        return -val;
    }
    else
    {
        return val;
    }
}
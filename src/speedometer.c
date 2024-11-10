#include "../include/speedometer.h"

volatile static uint16_t turns;

void speedometer_init(void)
{
    // Enable Clocks
    rcc_periph_clock_enable(RCC_DMA1);
    rcc_periph_clock_enable(RCC_TIM2);
    rcc_periph_clock_enable(RCC_TIM1);
    rcc_periph_clock_enable(RCC_GPIOA);

    gpio_set_mode(SPEEDOMETER_PORT, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, SPEEDOMETER_PIN);

    // Timer 2 External Clock
    timer_slave_set_mode(ENCODER_TIMER, TIM_SMCR_SMS_ECM1);
    timer_slave_set_trigger(ENCODER_TIMER, TIM_SMCR_TS_ETRF);
    timer_slave_set_polarity(ENCODER_TIMER, TIM_ET_RISING);
    timer_slave_set_filter(ENCODER_TIMER, TIM_IC_DTF_DIV_32_N_8);
    // timer_slave_set_prescaler(ENCODER_TIMER, PULSES_1TURN);  //Automaticaly computes to turns

    // Timer 1 send data CH1 - DMA_CH2
    timer_set_prescaler(TENMS_TIMER, MS_PS);
    timer_set_period(TENMS_TIMER, MS_INTERVAL); // Set period to 600ms
    timer_set_oc_value(TENMS_TIMER, TIM_OC1, 49999);
    timer_set_oc_mode(TENMS_TIMER, TIM_OC1, TIM_OCM_FROZEN);
    timer_enable_irq(TENMS_TIMER, TIM_DIER_CC1DE); // Enable the update DMA request

    // DMA Config
    dma_channel_reset(DMA1, DMA_CH);
    dma_set_priority(DMA1, DMA_CH, DMA_CCR_PL_VERY_HIGH);
    dma_set_peripheral_address(DMA1, DMA_CH, (uint32_t)&TIM2_CNT);
    dma_set_memory_address(DMA1, DMA_CH, (uint32_t)&turns);
    dma_set_number_of_data(DMA1, DMA_CH, 1);
    dma_set_peripheral_size(DMA1, DMA_CH, DMA_CCR_PSIZE_16BIT);
    dma_set_memory_size(DMA1, DMA_CH, DMA_CCR_MSIZE_16BIT);
    dma_enable_circular_mode(DMA1, DMA_CH);

    // Enable transfer complete interrupt to reset TIM2_CNT
    dma_enable_transfer_complete_interrupt(DMA1, DMA_CH);
    nvic_enable_irq(NVIC_DMA1_CHANNEL2_IRQ);
    dma_enable_channel(DMA1, DMA_CH);

    // Enable all counters
    timer_enable_counter(ENCODER_TIMER);
    timer_enable_counter(TENMS_TIMER);
}

void dma1_channel2_isr(void)
{
    if (dma_get_interrupt_flag(DMA1, DMA_CH, DMA_TCIF))
    {
        dma_clear_interrupt_flags(DMA1, DMA_CH, DMA_TCIF);
        timer_set_counter(ENCODER_TIMER, 0);
    }
}

float speedometer_getRPM(void)
{
    return (float)(turns * CONSTANT_TO_RPM);
}

float speedometer_getRAD_S(void)
{
    return (float)(turns * CONSTANT_TO_RAD_S);
}

#include "speedometer.h"

static volatile uint16_t turns[2]; /** Variable to store the number of encoder turns */

/**
 * Initialize the speedometer by configuring timers, DMA, and GPIO
 */
void speedometer_init(void)
{
    /** Enable clocks for DMA1, TIM2 (encoder timer), TIM1 (timer for measurements), and GPIOA */
    rcc_periph_clock_enable(RCC_DMA1);
    rcc_periph_clock_enable(RCC_TIM2);
    rcc_periph_clock_enable(RCC_TIM1);
    rcc_periph_clock_enable(RCC_GPIOA);

    /** Configure speedometer GPIO pin as floating input */
    gpio_set_mode(SPEEDOMETER_PORT, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, SPEEDOMETER_PIN);

    /** Configure TIM2 as an external clock source for the encoder */
    timer_slave_set_mode(ENCODER_TIMER, TIM_SMCR_SMS_ECM1);       /** Set timer to external clock mode */
    timer_slave_set_trigger(ENCODER_TIMER, TIM_SMCR_TS_ETRF);     /** Use an external trigger as the source */
    timer_slave_set_polarity(ENCODER_TIMER, TIM_ET_RISING);       /** Trigger on rising edges */
    timer_slave_set_filter(ENCODER_TIMER, TIM_IC_DTF_DIV_32_N_8); /** Set filter to reduce noise */
    // timer_slave_set_prescaler(ENCODER_TIMER, PULSES_1TURN);    /** Automaticaly computes to turns */

    /**
     * Configure TIM1 (TENMS_TIMER) to generate periodic DMA requests
     */
    timer_set_prescaler(TENMS_TIMER, MS_PS);                 /** Set prescaler to adjust timer clock speed */
    timer_set_period(TENMS_TIMER, MS_INTERVAL);              /** Set timer period to 600ms */
    timer_set_oc_value(TENMS_TIMER, TIM_OC1, 49999);         /** Set output compare value */
    timer_set_oc_mode(TENMS_TIMER, TIM_OC1, TIM_OCM_FROZEN); /** Freeze output when counter reaches OC1 value */
    timer_enable_irq(TENMS_TIMER, TIM_DIER_CC1DE);           /** Enable DMA request on update events */

    /**
     * Configure DMA1 Channel for reading TIM2 counter (encoder position)
     */
    dma_channel_reset(DMA1, DMA_CH);                      /** Reset DMA channel to clear previous configuration */
    dma_set_priority(DMA1, DMA_CH, DMA_CCR_PL_VERY_HIGH); /** Set high priority for DMA transfers */
    dma_set_peripheral_address(DMA1, DMA_CH, (uint32_t)&TIM2_CNT); /** Set DMA source to TIM2 counter register */
    dma_set_memory_address(DMA1, DMA_CH, (uint32_t)turns);         /** Set DMA destination to `turns` variable */
    dma_set_number_of_data(DMA1, DMA_CH, 2);                       /** Transfer a single data item per request */
    dma_set_peripheral_size(DMA1, DMA_CH, DMA_CCR_PSIZE_16BIT);    /** Set peripheral data size to 16 bits */
    dma_set_memory_size(DMA1, DMA_CH, DMA_CCR_MSIZE_16BIT);        /** Set memory data size to 16 bits */
    dma_enable_circular_mode(DMA1, DMA_CH);                        /** Enable circular mode for continuous updates */
    dma_enable_memory_increment_mode(DMA1, DMA_CH);

    /** Enable transfer complete interrupt to reset TIM2 counter after each DMA transfer */
    dma_enable_transfer_complete_interrupt(DMA1, DMA_CH); /** Enable interrupt on transfer completion */
    // nvic_enable_irq(NVIC_DMA1_CHANNEL2_IRQ);              /** Enable NVIC interrupt for DMA channel 2 */
    dma_enable_channel(DMA1, DMA_CH); /** Start the DMA channel */

    /** Enable all counters to begin measurements */
    timer_enable_counter(ENCODER_TIMER); /** Start TIM2 (encoder timer) */
    timer_enable_counter(TENMS_TIMER);   /** Start TIM1 (measurement timer) */
}

/**
 * Calculate and return the speed in RPM based on encoder turns
 */
float speedometer_getRPM(void)
{
    return (float)(abs(turns[1] - turns[0]) * CONSTANT_TO_RPM); /** Convert turns to RPM */
}

/**
 * Calculate and return the speed in radians per second based on encoder turns
 */
float speedometer_getRAD_S(void)
{
    return (float)(abs(turns[1] - turns[0]) * CONSTANT_TO_RAD_S); /** Convert turns to rad/s */
}

uint16_t abs(int16_t num)
{
    return (num < 0) ? -num : num;
}

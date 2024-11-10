#include "../include/setpoint.h"

volatile static uint16_t potBuff[N_DATA];

void pot_init(void)
{
    // Enables all peripheral clocks
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_ADC1);
    rcc_periph_clock_enable(RCC_DMA1);

    // Configure GPIO pin as analog input
    gpio_set_mode(POT_PORT, GPIO_MODE_INPUT, GPIO_CNF_INPUT_ANALOG, POT_PIN);

    // Configure DMA1 Channel 1 for ADC requests
    dma_channel_reset(DMA1, POT_DMA_CHANNEL);
    dma_set_priority(DMA1, POT_DMA_CHANNEL, DMA_CCR_PL_VERY_HIGH);
    dma_set_peripheral_address(DMA1, POT_DMA_CHANNEL, (uint32_t)&ADC_DR(ADC1));
    dma_set_memory_address(DMA1, POT_DMA_CHANNEL, (uint32_t)potBuff);
    dma_set_number_of_data(DMA1, POT_DMA_CHANNEL, N_DATA);
    dma_set_memory_size(DMA1, POT_DMA_CHANNEL, DMA_CCR_MSIZE_16BIT);
    dma_set_peripheral_size(DMA1, POT_DMA_CHANNEL, DMA_CCR_PSIZE_16BIT);
    dma_enable_circular_mode(DMA1, POT_DMA_CHANNEL);
    dma_enable_memory_increment_mode(DMA1, POT_DMA_CHANNEL);
    dma_set_read_from_peripheral(DMA1, POT_DMA_CHANNEL);
    dma_enable_channel(DMA1, POT_DMA_CHANNEL);

    // ADC setup: turn off ADC before configuring DMA
    adc_power_off(ADC1);
    adc_enable_dma(ADC1);
    adc_disable_eoc_interrupt(ADC1);
    adc_enable_scan_mode(ADC1);
    adc_disable_temperature_sensor();
    adc_set_continuous_conversion_mode(ADC1);
    adc_set_right_aligned(ADC1);


    // Set up ADC channel sequence and sample time
    const uint8_t ch[1] = {POT_ADC_CHANNEL};  // Use defined ADC channel
    adc_set_regular_sequence(ADC1, 1, ch);
    adc_set_sample_time(ADC1, POT_ADC_CHANNEL, SAMPLE_TIME_CYCLES);

    // Power on ADC and calibrate
    adc_power_on(ADC1);
    adc_reset_calibration(ADC1);
    adc_calibrate(ADC1);
    while(adc_is_calibrating(ADC1));
    adc_enable_external_trigger_regular(ADC1, ADC_CR2_EXTSEL_SWSTART);
    adc_start_conversion_regular(ADC1);

}

float pot_get_value(void)
{
    // Simple filter: average data in buffer
    uint32_t ac = 0;
    for(uint8_t i = 0; i < N_DATA; i++)
    {
        ac += potBuff[i];
    }
    return (float)ac * CONSTANT_TO_PERCENTAGE;
}

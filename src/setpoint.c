#include "setpoint.h"

static volatile uint16_t potBuff[N_DATA]; /** Buffer to store ADC readings for the potentiometer */

void pot_init(void)
{
    /** Enable peripheral clocks for GPIOB, ADC1, and DMA1 */
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_ADC1);
    rcc_periph_clock_enable(RCC_DMA1);

    /** Configure the potentiometer GPIO pin as an analog input */
    gpio_set_mode(POT_PORT, GPIO_MODE_INPUT, GPIO_CNF_INPUT_ANALOG, POT_PIN);

    /** Configure DMA1 Channel 1 for ADC requests */
    dma_channel_reset(DMA1, POT_DMA_CHANNEL);                      /** Reset DMA channel for fresh configuration */
    dma_set_priority(DMA1, POT_DMA_CHANNEL, DMA_CCR_PL_VERY_HIGH); /** Set highest priority for the DMA channel */
    dma_set_peripheral_address(
        DMA1, POT_DMA_CHANNEL, (uint32_t)&ADC_DR(ADC1)); /** Set the DMA source address to ADC data register */
    dma_set_memory_address(DMA1, POT_DMA_CHANNEL, (uint32_t)potBuff); /** Set the destination address to the buffer */
    dma_set_number_of_data(DMA1, POT_DMA_CHANNEL, N_DATA);            /** Set the number of data elements to transfer */
    dma_set_memory_size(DMA1, POT_DMA_CHANNEL, DMA_CCR_MSIZE_16BIT);  /** Configure memory data size as 16 bits */
    dma_set_peripheral_size(
        DMA1, POT_DMA_CHANNEL, DMA_CCR_PSIZE_16BIT);         /** Configure peripheral data size as 16 bits */
    dma_enable_circular_mode(DMA1, POT_DMA_CHANNEL);         /** Enable circular mode for continuous data transfer */
    dma_enable_memory_increment_mode(DMA1, POT_DMA_CHANNEL); /** Enable memory increment mode to move through buffer */
    dma_set_read_from_peripheral(DMA1, POT_DMA_CHANNEL);     /** Configure DMA to read from the peripheral (ADC) */
    dma_enable_channel(DMA1, POT_DMA_CHANNEL);               /** Enable the DMA channel */

    /** ADC setup: disable ADC to configure DMA settings */
    adc_power_off(ADC1);                      /** Turn off the ADC */
    adc_enable_dma(ADC1);                     /** Enable DMA mode in ADC */
    adc_disable_eoc_interrupt(ADC1);          /** Disable end-of-conversion interrupt */
    adc_enable_scan_mode(ADC1);               /** Enable scan mode for multi-channel if needed */
    adc_disable_temperature_sensor();         /** Disable internal temperature sensor channel */
    adc_set_continuous_conversion_mode(ADC1); /** Enable continuous conversion mode */
    adc_set_right_aligned(ADC1);              /** Set right alignment for ADC results */

    /** Set up the ADC channel sequence and sample time */
    uint8_t ch[1] = {POT_ADC_CHANNEL};     /** Define the ADC channel for the potentiometer */
    adc_set_regular_sequence(ADC1, 1, ch); /** Set ADC sequence to use only the specified channel */
    adc_set_sample_time(ADC1, POT_ADC_CHANNEL, SAMPLE_TIME_CYCLES); /** Set sample time for the channel */

    /** Power on the ADC and calibrate it */
    adc_power_on(ADC1);          /** Turn on the ADC */
    adc_reset_calibration(ADC1); /** Reset ADC calibration */
    adc_calibrate(ADC1);         /** Start ADC calibration */
    while (adc_is_calibrating(ADC1))
        ; /** Wait for calibration to complete */

    adc_enable_external_trigger_regular(ADC1,
                                        ADC_CR2_EXTSEL_SWSTART); /** Enable software trigger for conversion start */
    adc_start_conversion_regular(ADC1);                          /** Start the ADC conversions */
}

float pot_get_value(void)
{
    uint32_t ac = 0; /** Accumulator for averaging buffer values */

    /** Sum up all values in the buffer */
    for (uint8_t i = 0; i < N_DATA; i++)
    {
        ac += potBuff[i];
    }

    /** Return the average value, converted to a percentage */
    return (float)ac * CONSTANT_TO_PERCENTAGE;
}

/**
 * @file pot.h
 * @brief Potentiometer module for ADC with DMA on STM32.
 *
 * This module initializes an ADC to read values from a potentiometer using DMA.
 * It includes functions to initialize the potentiometer ADC and retrieve the ADC value.
 */

#include <libopencm3/stm32/adc.h>
#include <libopencm3/stm32/dma.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

/** @brief Number of data samples to collect via DMA. */
#define N_DATA 10

/** @brief Conversion constant to calculate potentiometer percentage value. */
#define CONSTANT_TO_PERCENTAGE (24.42e-3) / N_DATA

/** @brief ADC sample time configuration. */
#define SAMPLE_TIME_CYCLES ADC_SMPR_SMP_239DOT5CYC

/** @brief ADC used for potentiometer readings. */
#define POT_ADC ADC1

/** @brief ADC channel associated with the potentiometer input. */
#define POT_ADC_CHANNEL ADC_CHANNEL9

/** @brief DMA channel associated with the ADC. */
#define POT_DMA_CHANNEL DMA_CHANNEL1

/** @brief GPIO port where the potentiometer pin is connected. */
#define POT_PORT GPIOB

/** @brief GPIO pin where the potentiometer is connected. */
#define POT_PIN GPIO1

/**
 * @brief Initializes the ADC and DMA for potentiometer readings.
 *
 * This function configures the ADC and DMA to sample potentiometer values from the specified channel.
 * It sets up GPIO, enables the required clocks, configures the ADC sample time,
 * and initiates DMA for continuous data collection.
 */
void pot_init(void);

/**
 * @brief Retrieves the latest potentiometer value.
 *
 * This function returns the current potentiometer reading as an ADC value.
 * It should be called after `pot_init` has configured the ADC and DMA.
 *
 * @return The ADC value corresponding to the potentiometer position (0-4095 for 12-bit resolution).
 */
float pot_get_value(void);

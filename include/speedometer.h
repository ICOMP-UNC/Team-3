#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/dma.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>

//#define TIM4_CCR4_ADDRESS (TIM4 + 0x40) // TIM4 capture/compare register 4 address

#define N_PULSE             2 // High pulses for 1/4 turn
#define CONVERSION_CONSTANT 48

#define SPEEDOMETER_PIN  GPIO8
#define SPEEDOMETER_PORT GPIOB

void speedometer_init(void);
float speedometer_getRPM(void);
uint16_t abs(uint16_t val);
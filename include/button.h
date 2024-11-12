#include "libopencm3/stm32/exti.h"
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/cm3/nvic.h"

#define STOP_BUTTON_PORT GPIOB
#define STOP_BUTTON_PIN GPIO10
#define CONTROL_BUTTON_PORT GPIOB
#define CONTROL_BUTTON_PIN GPIO11

#define DEBOUNCE_DELAY 200  //Delay for Unbounce in [ms]

void button_init(void);

uint8_t button_get_stop_flag(void);

uint8_t button_get_object_flag(void);

void button_set_object_flag(uint8_t boolean);

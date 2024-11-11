#include "libopencm3/stm32/exti.h"
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/cm3/nvic.h"

#define STOP_BUTTON_PORT GPIOB
#define STOP_BUTTON_PIN GPIO10
#define CONTROL_BUTTON_PORT GPIOB
#define CONTROL_BUTTON_PIN GPIO11

#define HEIGHT_TRESHOLD 10

void gpio_pin_setup();

void exti_setup();

void exti10_isr();

void exti11_isr();

uint8_t get_item_height();

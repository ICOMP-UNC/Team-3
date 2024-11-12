#include "../include/button.h"
#include "../include/motor_driver.h"
#include "libopencm3/cm3/systick.h"

static volatile uint8_t object_flag = 0; // This Flag is used to display object data
static volatile uint8_t stop_flag = 0;   // This Flag is used externaly to display Data (update)
static volatile uint32_t last_exti10_time;

void button_init(void)
{
    rcc_periph_clock_enable(RCC_AFIO);
    rcc_periph_clock_enable(RCC_GPIOB);

    // Configurar STOP_BUTTON (PB11) como entrada con pull-up
    gpio_set_mode(STOP_BUTTON_PORT, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, STOP_BUTTON_PIN);

    // Configurar CONTROL_BUTTON (PB10) como entrada con pull-up
    gpio_set_mode(CONTROL_BUTTON_PORT, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, CONTROL_BUTTON_PIN);

    // Configuración de prioridad y habilitación en NVIC
    nvic_set_priority(NVIC_EXTI15_10_IRQ, 0); // Máx
    nvic_enable_irq(NVIC_EXTI15_10_IRQ);

    // Configurar EXTI10 (PB10)
    exti_select_source(EXTI10, STOP_BUTTON_PORT);
    exti_set_trigger(EXTI10, EXTI_TRIGGER_FALLING); // Object Detection

    // Configurar EXTI11 (PB11)
    exti_select_source(EXTI11, CONTROL_BUTTON_PORT);
    exti_set_trigger(EXTI11, EXTI_TRIGGER_RISING); // STOP BUTTON

    exti_enable_request(EXTI10);
    exti_enable_request(EXTI11);
}

void exti15_10_isr(void)
{
    uint32_t current_time = systick_get_value();
    // STOP_BUTTON Handler
    if (exti_get_flag_status(EXTI11))
    {

        exti_reset_request(EXTI11);

        stop_flag = 1;
        // Stop-Start the motor as fast as possible
        motor_disable();
    }

    // CONTROL_BUTTON Handler
    if (exti_get_flag_status(EXTI10))
    {
        // Unbounce Logic
        if ((current_time - last_exti10_time) > DEBOUNCE_DELAY)
        {
            exti_reset_request(EXTI10);
            last_exti10_time = current_time;
            object_flag = 1;
        }
    }
}

uint8_t button_get_stop_flag()
{
    return stop_flag;
}

uint8_t button_get_object_flag()
{
    return object_flag;
}

void button_set_object_flag(uint8_t boolean)
{
    object_flag = boolean;
}

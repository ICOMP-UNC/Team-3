#include "../include/exti.h"
#include "../include/hc_sr04.h"
#include "../include/motor_driver.h"

uint8_t flag_item_height = 0;

void exti_setup(void)
{
    rcc_periph_clock_enable(RCC_AFIO);
    rcc_periph_clock_enable(RCC_GPIOB);

    // Configurar STOP_BUTTON (PB10) como entrada con pull-up
    gpio_set_mode(STOP_BUTTON_PORT, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, STOP_BUTTON_PIN);

    // Configurar CONTROL_BUTTON (PB11) como entrada con pull-up
    gpio_set_mode(CONTROL_BUTTON_PORT, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, CONTROL_BUTTON_PIN);

    // Configuración de prioridad y habilitación en NVIC
    nvic_set_priority(NVIC_EXTI15_10_IRQ, 0);   //Máx
    nvic_enable_irq(NVIC_EXTI15_10_IRQ);

    // Configurar EXTI10 (PB10)
    exti_select_source(EXTI10, STOP_BUTTON_PORT);
    exti_set_trigger(EXTI10, EXTI_TRIGGER_RISING); // Cambiar a EXTI_TRIGGER_BOTH si quieres ambas detecciones

    // Configurar EXTI11 (PB11)
    exti_select_source(EXTI11, CONTROL_BUTTON_PORT);
    exti_set_trigger(EXTI11, EXTI_TRIGGER_BOTH);

    exti_enable_request(EXTI10);
    exti_enable_request(EXTI11);
}

void exti15_10_isr(void)
{
    // Manejo de EXTI10 (PB10 - STOP_BUTTON)
    if (exti_get_flag_status(EXTI10)) {
        exti_reset_request(EXTI10);
        if (motor_get_state()) {
            motor_disable();
        } else {
            motor_enable();
        }
    }

    // Manejo de EXTI11 (PB11 - CONTROL_BUTTON)
    if (exti_get_flag_status(EXTI11)) {
        exti_reset_request(EXTI11);

        // Si el pin está bajo, lee distancia
        if (!gpio_get(CONTROL_BUTTON_PORT, CONTROL_BUTTON_PIN)) {
            int height = hcsr04_get_distance();
            if (height < HEIGHT_THRESHOLD) {
                motor_disable();
                flag_item_height = 1;
            } else {
                flag_item_height = 0;
            }
        } else {
            motor_enable();
        }
    }
}

uint8_t get_item_height(void)
{
    return flag_item_height;
}

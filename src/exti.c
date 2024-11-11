#include "../include/exti.h"
#include "../include/hc_sr04.h"
#include "../include/motor_driver.h"

uint8_t flag_item_height = 0;

void gpio_pin_setup()
{
    // Habilitar el reloj para el puerto GPIOA
    rcc_periph_clock_enable(RCC_GPIOA);

    // Configurar STOP_BUTTON como entrada con pull-up
    gpio_set_mode(STOP_BUTTON_PORT, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, STOP_BUTTON_PIN);

    // Configurar CONTROL_BUTTON como entrada con pull-up
    gpio_set_mode(CONTROL_BUTTON_PORT, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, CONTROL_BUTTON_PIN);
}

void exti_setup()
{
    rcc_periph_clock_enable(RCC_AFIO);

    nvic_enable_irq(NVIC_EXTI15_10_IRQ);

    exti_select_source(EXTI10, STOP_BUTTON_PORT);  
    exti_set_trigger(EXTI10, EXTI_TRIGGER_RISING);
    exti_enable_request(EXTI10);       

    exti_select_source(EXTI11, CONTROL_BUTTON_PORT); 
    exti_set_trigger(EXTI11, EXTI_TRIGGER_BOTH); 
    exti_enable_request(EXTI11);    
}

void exti10_isr()
{
    exti_reset_request(EXTI10);

    if (get_motor_state())
    {
        motor_disable();
    }
    else
    {
        motor_enable();
    } 
}

void exti11_isr()
{
    exti_reset_request(EXTI11);

    // Detectar el estado actual del pin del botón de control
    if (!gpio_get(CONTROL_BUTTON_PORT, CONTROL_BUTTON_PIN))
    {
        int height = hcsr04_get_distance();

        if (height < HEIGHT_TRESHOLD)
        {
            motor_disable();
            flag_item_height = 1;
        }
        else
        {
            flag_item_height = 0;
        }
    }
    else
    {
        motor_enable();
    }
}

uint8_t get_item_height()
{
    return flag_item_height;
}

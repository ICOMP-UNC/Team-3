#include "button.h"
#include "libopencm3/cm3/systick.h"
#include "motor_driver.h"

static volatile uint8_t object_flag = 0;       /**< Flag used to indicate when object data should be displayed. */
static volatile uint8_t stop_flag = 0;         /**< Flag used externally to control the data display (update). */
static volatile uint32_t last_exti10_time = 0; /**< Last recorded time for EXTI10, used for debouncing logic. */

void button_init(void)
{
    rcc_periph_clock_enable(RCC_AFIO);  /**< Enable clock for Alternate Function I/O. */
    rcc_periph_clock_enable(RCC_GPIOB); /**< Enable clock for GPIOB. */

    // Configure STOP_BUTTON (PB11) as input with pull-up
    gpio_set_mode(STOP_BUTTON_PORT, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, STOP_BUTTON_PIN);

    // Configure CONTROL_BUTTON (PB10) as input with pull-up
    gpio_set_mode(CONTROL_BUTTON_PORT, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, CONTROL_BUTTON_PIN);

    // Set priority and enable EXTI15_10 interrupt in NVIC
    nvic_set_priority(NVIC_EXTI15_10_IRQ, 0); /**< Set EXTI15_10 interrupt priority to highest (0). */
    nvic_enable_irq(NVIC_EXTI15_10_IRQ);      /**< Enable EXTI15_10 interrupt in NVIC. */

    // Configure EXTI10 (PB10) as falling edge trigger for object detection
    exti_select_source(EXTI10, STOP_BUTTON_PORT);   /**< Select GPIO port for EXTI10 (PB10). */
    exti_set_trigger(EXTI10, EXTI_TRIGGER_FALLING); /**< Set EXTI10 trigger to falling edge. */

    // Configure EXTI11 (PB11) as rising edge trigger for STOP button
    exti_select_source(EXTI11, CONTROL_BUTTON_PORT); /**< Select GPIO port for EXTI11 (PB11). */
    exti_set_trigger(EXTI11, EXTI_TRIGGER_RISING);   /**< Set EXTI11 trigger to rising edge. */

    exti_enable_request(EXTI10); /**< Enable interrupt request for EXTI10. */
    exti_enable_request(EXTI11); /**< Enable interrupt request for EXTI11. */
}

void exti15_10_isr(void)
{
    uint32_t current_time = systick_get_value(); /**< Get current time for debouncing logic. */

    // STOP_BUTTON Handler (EXTI11)
    if (exti_get_flag_status(EXTI11))
    {
        exti_reset_request(EXTI11); /**< Clear EXTI11 interrupt flag. */
        stop_flag = 1;              /**< Set stop_flag to indicate a stop request. */
        motor_disable();            /**< Stop the motor immediately. */
    }

    // CONTROL_BUTTON Handler (EXTI10)
    if (exti_get_flag_status(EXTI10) && debounce(current_time, &last_exti10_time))
    {
        exti_reset_request(EXTI10); /**< Clear EXTI10 interrupt flag after debounce check passes. */
        object_flag = 1;            /**< Set `object_flag` to indicate object detection. */
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

static inline int debounce(uint32_t current_time, uint32_t* last_time)
{
    if ((current_time - *last_time) > DEBOUNCE_DELAY)
    {
        *last_time = current_time; /**< Update last recorded time for the interrupt line. */
        return 1;                  /**< Return true to indicate the interrupt is valid. */
    }
    return 0; /**< Return false if debounce delay has not passed. */
}

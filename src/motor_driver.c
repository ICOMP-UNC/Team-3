#include "motor_driver.h"

static uint8_t motor_state = 0; /**< Motor state: 1 indicates enabled, 0 indicates disabled */

void motor_init()
{
    rcc_periph_clock_enable(RCC_GPIOB); /**< Enable clock for GPIOB */
    rcc_periph_clock_enable(RCC_TIM3);  /**< Enable clock for TIM3 */

    gpio_set_mode(MOTOR_PORT, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, MOTOR_PIN); /**< Set MOTOR_PIN as 50 MHz alternate function push-pull output */

    timer_set_mode(MOTOR_TIMER, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP); /**< Configure TIMER in up-counting mode with auto-reload */
    timer_enable_preload(MOTOR_TIMER); /**< Enable preload for the timer */

    timer_set_prescaler(MOTOR_TIMER, 72 - 1); /**< Set prescaler to 72-1 for a 1 MHz tick frequency */
    timer_set_period(MOTOR_TIMER, MOTOR_PERIOD_MS * MS_TO_SEC - 1); /**< Set PWM period based on MOTOR_PERIOD_MS */

    timer_disable_oc_output(MOTOR_TIMER, TIM_OC3); /**< Disable output compare on TIM3 Channel 3 */
    timer_set_oc_mode(MOTOR_TIMER, TIM_OC3, TIM_OCM_PWM1); /**< Set output compare mode to PWM1 on TIM3 Channel 3 */
    timer_set_oc_value(MOTOR_TIMER, TIM_OC3, (MOTOR_PERIOD_MS * MS_TO_SEC) / 100); /**< Set duty cycle to 1% */
    motor_enable(); /**< Enable motor output */

    timer_enable_counter(MOTOR_TIMER); /**< Start the timer counter */
}

void motor_set_power(uint8_t percentage)
{
    if (motor_state) /**< Set power only if the motor is enabled */
    {
        if (percentage > 100) percentage = 100; /**< Cap percentage at 100 to avoid exceeding maximum duty cycle */
        timer_set_oc_value(MOTOR_TIMER, TIM_OC3, (MOTOR_PERIOD_MS * MS_TO_SEC * percentage) / 100); /**< Update duty cycle based on specified percentage */
    }
}

void motor_disable()
{
    motor_state = 0; /**< Set motor state to disabled */
    timer_disable_oc_output(MOTOR_TIMER, TIM_OC3); /**< Disable PWM output on TIM3 Channel 3 */
}

void motor_enable()
{
    motor_state = 1; /**< Set motor state to enabled */
    timer_enable_oc_output(MOTOR_TIMER, TIM_OC3); /**< Enable PWM output on TIM3 Channel 3 */
}

uint8_t motor_get_state()
{
    return motor_state; /**< Return the current motor state */
}

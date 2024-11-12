#include "motor_driver.h"

static uint8_t motor_state = MOTOR_DISABLED; /** Variable to store the motor's current state (enabled/disabled) */

/**
 * Initialize motor settings, configure GPIO, and set up the timer in PWM mode
 */
void motor_init()
{
    /** Enables clocks for GPIOB and TIM3 */
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_TIM3);

    /** Set GPIO mode for the motor pin to alternate function with push-pull at 50 MHz */
    gpio_set_mode(MOTOR_PORT, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, MOTOR_PIN);

    /** Configure TIM3 for up-counting mode and enable auto-reload */
    timer_set_mode(MOTOR_TIMER, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    timer_enable_preload(MOTOR_TIMER);

    /** Set prescaler to make the timer tick at 1 MHz */
    timer_set_prescaler(MOTOR_TIMER, 72 - 1);

    /** Set the timer period to match the desired motor PWM period in milliseconds */
    timer_set_period(MOTOR_TIMER, MOTOR_PERIOD_MS * 1000 - 1);

    /** Configure PWM output capture on the motor pin */
    timer_disable_oc_output(MOTOR_TIMER, TIM_OC3);         /** Disable output before setting up */
    timer_set_oc_mode(MOTOR_TIMER, TIM_OC3, TIM_OCM_PWM1); /** Set PWM mode 1 on the output */

    /** Set initial duty cycle to 1% */
    timer_set_oc_value(MOTOR_TIMER, TIM_OC3, (MOTOR_PERIOD_MS * 1000) / 100);

    motor_enable(); /** Enable motor output */

    timer_enable_counter(MOTOR_TIMER); /** Start the timer */
}

/**
 * Set motor power as a percentage (0-100), enabling the motor if it is not active
 */
void motor_set_power(uint8_t percentage)
{
    /** Enable the motor if it is currently disabled */
    if (!motor_state)
    {
        motor_enable();
    }

    /** Limit the power percentage to a maximum of 100 */
    if (percentage > 100)
        percentage = 100;

    /** Set the output capture value for the given percentage to adjust duty cycle */
    timer_set_oc_value(MOTOR_TIMER, TIM_OC3, (MOTOR_PERIOD_MS * 1000 * percentage) / 100);
}

/**
 * Disable motor output and set motor state to 0 (off)
 */
void motor_disable()
{
    motor_state = MOTOR_DISABLED;                  /** Update motor state to disabled */
    timer_disable_oc_output(MOTOR_TIMER, TIM_OC3); /** Disable PWM output */
}

/**
 * Enable motor output and set motor state to 1 (on)
 */
void motor_enable()
{
    motor_state = MOTOR_ENABLED;                  /** Update motor state to enabled */
    timer_enable_oc_output(MOTOR_TIMER, TIM_OC3); /** Enable PWM output */
}

uint8_t get_motor_state()
{
    return motor_state;
}

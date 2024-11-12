#include "hc_sr04.h"

static volatile uint32_t times[2];           /** Array to store the timer values for rising and falling edges */
static volatile float distance = 0;          /** Distance in centimeters */
static volatile uint8_t conversion_flag = 0; /** Flag to indicate completion of distance measurement */

void hcsr04_init(void)
{
    /** Enable clocks for GPIOB and TIM4 */
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_TIM4);

    /** Configure TRIG_PIN as a 2 MHz output push-pull pin for triggering the sensor */
    gpio_set_mode(HCSR04_PORT, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, TRIG_PIN);
    gpio_clear(HCSR04_PORT, TRIG_PIN); /** Ensure TRIG_PIN is low initially */

    /** Configure ECHO_PIN as a floating input pin to receive the echo signal */
    gpio_set_mode(HCSR04_PORT, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, ECHO_PIN);

    /** -- Configure TIM4 for input capture on channel 4, detecting both rising and falling edges -- */
    /** Set up TIM4 Channel 4 to capture on the rising edge */
    timer_ic_set_input(TIM4, TIM_IC4, TIM_IC_IN_TI4);    /** Map TIM4 CH4 to GPIOB9 */
    timer_ic_set_polarity(TIM4, TIM_IC4, TIM_IC_RISING); /** Rising edge for CH4 */
    timer_ic_enable(TIM4, TIM_IC4);                      /** Enable input capture on CH4 */

    /** Set up TIM4 Channel 3 to capture on the falling edge */
    timer_ic_set_input(TIM4, TIM_IC3, TIM_IC_IN_TI4);     /** Map TIM4 CH3 to GPIOB9 */
    timer_ic_set_polarity(TIM4, TIM_IC3, TIM_IC_FALLING); /** Falling edge for CH3 */
    timer_ic_enable(TIM4, TIM_IC3);                       /** Enable input capture on CH3 */

    /** Set the timer prescaler to create a 1 µs tick (based on APB1 frequency) */
    timer_set_prescaler(HCSR04_TIMER, (rcc_apb1_frequency / MHZ_DIVISOR) - 1);
    timer_enable_counter(HCSR04_TIMER); /** Start the timer */

    /** Enable interrupts for both capture channels */
    timer_enable_irq(HCSR04_TIMER, TIM_DIER_CC3IE | TIM_DIER_CC4IE);
    nvic_enable_irq(NVIC_TIM4_IRQ); /** Enable NVIC interrupt for TIM4 */
}

void hcsr04_trigger(void)
{
    gpio_set(HCSR04_PORT, TRIG_PIN);                  /** Set TRIG_PIN high to start the pulse */
    for (volatile uint8_t i = 0; i < DELAY_10US; i++) /** Delay for approximately 10 µs */
    {
        __asm__("nop"); /** No-operation for precise timing */
    }
    gpio_clear(HCSR04_PORT, TRIG_PIN); /** Set TRIG_PIN low to end the pulse */
}

void tim4_isr(void)
{
    /** Check if the rising edge capture flag is set */
    if (timer_get_flag(HCSR04_TIMER, TIM_SR_CC4IF))
    {
        timer_clear_flag(HCSR04_TIMER, TIM_SR_CC4IF); /** Clear the rising edge flag */
        times[0] = timer_get_counter(HCSR04_TIMER);   /** Record the rising edge time */
        conversion_flag = 0;                          /** Reset conversion flag for new measurement */
    }
    else
    {
        timer_clear_flag(HCSR04_TIMER, TIM_SR_CC3IF);             /** Clear the falling edge flag */
        times[1] = timer_get_counter(HCSR04_TIMER);               /** Record the falling edge time */
        distance = ((times[1] - times[0])) / SOUND_SPEED_DIVISOR; /** Calculate distance in cm */
        conversion_flag = 1; /** Set flag to indicate distance measurement is ready */
    }
}

float hcsr04_get_distance(void)
{
    hcsr04_trigger();          /** Start a new measurement by triggering the sensor */
    uint32_t timeout = 0xFFFF; // Set a timeout limit
    while (!conversion_flag && --timeout)
        ; /** Wait until the measurement is complete */
    if (!timeout)
    {
        return -1.0f;
    }
    return saturation(); /** Return the measured distance with saturation applied */
}

float saturation(void)
{
    if (distance > MAX_CAP_DISTANCE)
    {
        return MAX_CAP_DISTANCE; /** Cap distance at 400 cm if exceeded */
    }
    else if (distance <= MIN_CAP_DISTANCE)
    {
        return MIN_CAP_DISTANCE; /** Cap distance at 2 cm minimum */
    }
    else
    {
        return distance; /** Return the measured distance if within range */
    }
}

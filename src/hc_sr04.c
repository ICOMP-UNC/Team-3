#include "hc_sr04.h"

static volatile uint32_t times[2];           /** Array to store the timer values for rising and falling edges */
static volatile float distance = 0;          /** Distance in centimeters */
static volatile uint8_t conversion_flag = 0; /** Flag to indicate completion of distance measurement */

/**
 * Initialize the HC-SR04 ultrasonic sensor by configuring GPIO, timer, and interrupts
 */
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

    /**
     * Configure TIM4 for input capture on channel 4, detecting both rising and falling edges
     */
    timer_ic_set_input(HCSR04_TIMER, TIM_IC1, TIM_IC_IN_TI4);    /** Set TIM4 CH1 to capture on rising edge */
    timer_ic_set_polarity(HCSR04_TIMER, TIM_IC1, TIM_IC_RISING); /** Detect rising edge for start time */
    timer_ic_enable(HCSR04_TIMER, TIM_IC1);                      /** Enable input capture on CH1 */

    timer_ic_set_input(HCSR04_TIMER, TIM_IC2, TIM_IC_IN_TI4);     /** Set TIM4 CH2 to capture on falling edge */
    timer_ic_set_polarity(HCSR04_TIMER, TIM_IC2, TIM_IC_FALLING); /** Detect falling edge for end time */
    timer_ic_enable(HCSR04_TIMER, TIM_IC2);                       /** Enable input capture on CH2 */

    /** Set the timer prescaler to create a 1 µs tick (based on APB1 frequency) */
    timer_set_prescaler(HCSR04_TIMER, (rcc_apb1_frequency / MHZ_DIVISOR) - 1);
    timer_enable_counter(HCSR04_TIMER); /** Start the timer */

    /** Enable interrupts for both capture channels */
    timer_enable_irq(HCSR04_TIMER, TIM_DIER_CC1IE | TIM_DIER_CC2IE);
    nvic_enable_irq(NVIC_TIM4_IRQ); /** Enable NVIC interrupt for TIM4 */
}

/**
 * Trigger the HC-SR04 sensor by sending a 10 µs pulse on TRIG_PIN
 */
void hcsr04_trigger(void)
{
    gpio_set(GPIOA, TRIG_PIN);                /** Set TRIG_PIN high to start the pulse */
    for (volatile uint8_t i = 0; i < 30; i++) /** Delay for approximately 10 µs */
    {
        __asm__("nop"); /** No-operation for precise timing */
    }
    gpio_clear(GPIOA, TRIG_PIN); /** Set TRIG_PIN low to end the pulse */
}

/**
 * TIM4 interrupt service routine to capture echo signal times and calculate distance
 */
void tim4_isr(void)
{
    /** Check if the rising edge capture flag is set */
    if (timer_get_flag(HCSR04_TIMER, TIM_SR_CC1IF))
    {
        timer_clear_flag(HCSR04_TIMER, TIM_SR_CC1IF); /** Clear the rising edge flag */
        times[0] = timer_get_counter(HCSR04_TIMER);   /** Record the rising edge time */
        conversion_flag = 0;                          /** Reset conversion flag for new measurement */
    }
    else
    {
        timer_clear_flag(HCSR04_TIMER, TIM_SR_CC2IF);             /** Clear the falling edge flag */
        times[1] = timer_get_counter(HCSR04_TIMER);               /** Record the falling edge time */
        distance = ((times[1] - times[0])) / SOUND_SPEED_DIVISOR; /** Calculate distance in cm */
        conversion_flag = 1; /** Set flag to indicate distance measurement is ready */
    }
}

/**
 * Trigger a measurement and return the calculated distance in cm
 */
float hcsr04_get_distance(void)
{
    hcsr04_trigger(); /** Start a new measurement by triggering the sensor */
    while (!conversion_flag)
        ;                /** Wait until the measurement is complete */
    return saturation(); /** Return the measured distance with saturation applied */
}

/**
 * Apply saturation limits to the distance measurement
 * @return Distance capped between 2 cm and 400 cm
 */
float saturation(void)
{
    if (distance > 400.0)
    {
        return 400.0; /** Cap distance at 400 cm if exceeded */
    }
    else if (distance <= 2.0)
    {
        return 2.0; /** Cap distance at 2 cm minimum */
    }
    else
    {
        return distance; /** Return the measured distance if within range */
    }
}

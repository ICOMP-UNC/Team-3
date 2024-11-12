#include "update.h"

static volatile float speed = 0;            /**< Current speed in RPM. */
static volatile float set = 0;              /**< Current SetPoint in RPM. */
static volatile float measurement_prom = 0; /**< Average of the distance measurements. */

static volatile uint16_t response__pid_count = 0;         // Count for PID Response Time
static volatile uint16_t response__measurement_count = 0; // Count for MEASUREMENT Response Time
static volatile uint16_t response__display_count = 0;     // Count for DISPLAY Response Time

static volatile uint8_t measure_done_flag = 0, pass_flag = 0;
static volatile uint8_t measure_count = 0;
static volatile float measurements[N_MEASUREMENT]; /**< Buffer for distance measurements. */

void update_init(void)
{
    systick_set_reload(TICKS_FOR_MS - 1);           /**< Set reload value for 1 ms. */
    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB); /**< Use the AHB clock as the SysTick source. */
    systick_counter_enable();                       /**< Enable the SysTick counter. */
    systick_interrupt_enable();                     /**< Enable SysTick interrupt. */
}

/**
 * @brief SysTick interrupt handler.
 *
 * Updates PID control, performs distance measurements, and updates the LCD display
 * based on defined response rates. The handler handles various system events based on
 * timing counters and flag conditions.
 */
void sys_tick_handler(void)
{
    systick_get_countflag(); /**< Clears the interrupt flag by reading the count flag. */

    if (button_get_stop_flag()) // Stopped
    {
        systick_counter_disable();
        lcd_clear();
        lcd_print_string("    STOPPED!    "); /**< Display "STOPPED!" if motor is stopped. */
        lcd_set_cursor(2, 0);                 /**< Move cursor to the second row for setpoint display. */
        lcd_print_string("Please Restart ");
    }
    else // Not Stopped
    {
        if (button_get_object_flag()) // Object
        {
            motor_disable();
            if (response__measurement_count >= MEASUREMENT_RATE)
            {
                response__measurement_count = 0;
                measure();
            }
            if (response__display_count >= DISPLAY_RATE/2)
            {
                response__display_count = 0;
                display_measure_info();
            }
            response__measurement_count++;
            response__display_count++;
        }
        else // No object
        {
            motor_enable();
            response__measurement_count = 0;
            if (response__pid_count >= PID_RATE)
            {
                response__pid_count = 0;
                upt_pid();
            }
            if (response__display_count >= DISPLAY_RATE)
            {
                response__display_count = 0;
                display_speed();
            }
            response__pid_count++;
            response__display_count++;
        }
    }
}

float get_measurement_prom(void)
{
    float ac = 0;
    for (uint8_t i = 0; i < N_MEASUREMENT; i++)
    {
        ac += measurements[i];
    }
    return ac / N_MEASUREMENT;
}

void display_speed(void)
{
    lcd_clear();
    lcd_print_string("RPM: ");                /**< Display "RPM :" on the LCD. */
    lcd_print_string(float_to_string(speed)); /**< Display the current speed in RPM. */
    lcd_set_cursor(2, 0);                     /**< Move cursor to the second row for setpoint display. */
    lcd_print_string("Target: ");             /**< Display "Target:" label for setpoint. */
    lcd_print_string(float_to_string(set));   /**< Display the current setpoint in RPM. */
}

void measure(void)
{
    if (!measure_done_flag)
    {
        measurements[measure_count] = hcsr04_get_distance(); /**< Stores distance in measurement buffer. */
        measure_count++;

        if (measure_count == N_MEASUREMENT)
        {
            measure_count = 0;
            measure_done_flag = 1;
            measurement_prom = get_measurement_prom();                  /**< Calculates average measurement. */
            pass_flag = (MEASUREMENT_TRHS <= measurement_prom) ? 1 : 0; /**< Sets pass or fail based on threshold. */
            if(!pass_flag)
            {
                lcd_clear();
                lcd_print_string("NOT PASS : ");                        /**< Indicate measurement did not pass. */
                lcd_print_string(float_to_string(measurement_prom)); /**< Display the failed measurement. */
                systick_counter_disable(); // Stop The system and restart when object is not there anymore
            }
        }
    }
}

void upt_pid(void)
{
    set = pot_get_value() * MAX_RPM / 100;
    pid_setpoint(set);                  /**< Updates setpoint based on potentiometer input. */
    speed = speedometer_getRPM();       /**< Retrieves the current speed in RPM. */
    motor_set_power(pid_update(speed)); /**< Updates motor power based on PID output with current speed. */
}

void display_measure_info(void)
{
    lcd_clear();
    if (measure_done_flag)
    {
        lcd_print_string("Height: ");                                       /**< Display "Measurement:" on the LCD. */
        lcd_print_string(float_to_string(get_measurement_prom()));          /**< Display measurement value. */
        for(volatile uint32_t i = 0; i < DELAY3_S; i++) __asm__("nop");     //Wait 3 Seconds
        button_set_object_flag(0);                                          // Restart Flag
        measure_done_flag = 0;                                              // Restart For New Measures
    }
    else // Display Percentage
    {
        lcd_print_string("Measuring height"); /**< Indicate object measurement in progress. */
        lcd_set_cursor(2, 0);                 /**< Move cursor to the second row for setpoint display. */
        lcd_print_string(float_to_string(measure_count * 10));
        lcd_print_string("/100");
    }
}

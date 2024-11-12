#include "update.h"

volatile static float speed = 0;            /**< Current filtered speed in RPM. */
volatile static uint8_t response_count = 0; /**< Count for the system PID to response. */

volatile static uint8_t measure_done_flag = 0, pass_flag;
volatile static uint8_t measure_count = 0;
volatile static float measurements[N_MEASUREMENT]; /**< Buffer for distance measurements. */
volatile static float measurement_prom = 0;        /**< Average of the distance measurements. */

void systick_init(void)
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

    if (response_count == PID_RATE && !button_get_stop_flag()) // Update PID if Not Stopped
    {
        pid_setpoint(pot_get_value() * MAX_RPM / 100); /**< Updates setpoint based on potentiometer input. */
        speed = speedometer_getRPM();                  /**< Retrieves the current speed in RPM. */
        motor_set_power(pid_update(speed)); /**< Updates motor power based on PID output with current speed. */
    }
    else if (response_count == MEASUREMENT_RATE && button_get_object_flag()) // Measure Object if object detected
    {
        measurements[measure_count] = hcsr04_get_distance(); /**< Stores distance in measurement buffer. */
        measure_count++;

        if (measure_count == N_MEASUREMENT)
        {
            measure_count = 0;
            measure_done_flag = 1;
            measurement_prom = get_measurement_prom(); /**< Calculates average measurement. */

            pass_flag = (MEASUREMENT_TRHS <= measurement_prom) ? 0 : 1; /**< Sets pass or fail based on threshold. */
        }
    }
    else if (response_count == DISPLAY_RATE) // Update LCD
    {
        response_count = 0;
        lcd_clear();

        if (!button_get_stop_flag())
        {
            lcd_print_string("STOPPED!!!"); /**< Display "STOPPED!!!" if motor is stopped. */
        }
        else
        {
            if (!button_get_object_flag())
            {
                lcd_print_string("Speed: ");              /**< Display "Speed:" on the LCD. */
                lcd_print_string(float_to_string(speed)); /**< Display the current speed in RPM. */
                lcd_set_cursor(2, 0);                     /**< Move cursor to the second row for setpoint display. */
                lcd_print_string("Target: ");             /**< Display "Target:" label for setpoint. */
                lcd_print_string(float_to_string(set));   /**< Display the current setpoint in RPM. */
            }
            else
            {
                if (measure_done_flag)
                {
                    lcd_print_string("Getting Object Info..."); /**< Indicate object measurement in progress. */
                }
                else
                {
                    if (pass_flag)
                    {
                        lcd_print_string("Measurement: "); /**< Display "Measurement:" on the LCD. */
                        lcd_print_string(float_to_string(get_measurement_prom())); /**< Display measurement value. */
                    }
                    else
                    {
                        lcd_print_string("NOT PASS");                        /**< Indicate measurement did not pass. */
                        lcd_print_string(float_to_string(measurement_prom)); /**< Display the failed measurement. */
                    }
                }
            }
        }
    }
}

uint32_t get_system_time_ms(void)
{
    return systick_get_value();
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

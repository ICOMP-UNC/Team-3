#include "update.h"

static volatile float speed = 0; /**< Current filtered speed in RPM. */
static volatile float set = 0;
static volatile float measurement_prom = 0;  /**< Average of the distance measurements. */
static volatile uint16_t response_count = 0; // Count Systick Response Time

static volatile uint8_t measure_done_flag = 0, pass_flag;
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

    switch (response_count)
    {
        case PID_RATE:
            if (!button_get_stop_flag())
                upt_pid();
            break;
        case MEASUREMENT_RATE:
            if (button_get_object_flag())
                measure();          //TODO: Agregar flag cuando saco el objeto mientras se mide
            break;
        case DISPLAY_RATE:
            response_count = 0;
            lcd_clear();
            if (!button_get_stop_flag()) // Not Stopped
            {
                if (!button_get_object_flag()) // No Object
                {
                    display_speed();
                }
                else // Object
                {
                    display_measurement();
                }
            }
            else
            {
                response_count == 0;
                lcd_clear();
                lcd_print_string("STOPPED!!!"); /**< Display "STOPPED!!!" if motor is stopped. */
            }
            break;
    }
    response_count++;
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
    lcd_print_string("Speed: ");              /**< Display "Speed:" on the LCD. */
    lcd_print_string(float_to_string(speed)); /**< Display the current speed in RPM. */
    lcd_set_cursor(2, 0);                     /**< Move cursor to the second row for setpoint display. */
    lcd_print_string("Target: ");             /**< Display "Target:" label for setpoint. */
    lcd_print_string(float_to_string(set));   /**< Display the current setpoint in RPM. */
}

void display_measurement(void)
{
    if (!measure_done_flag) // Measure Not Done
    {
        lcd_print_string("Getting Object Info..."); /**< Indicate object measurement in progress. */
    }
    else // Measure Done
    {
        if (pass_flag) // Measure Pass
        {
            lcd_print_string("Measurement: ");                         /**< Display "Measurement:" on the LCD. */
            lcd_print_string(float_to_string(get_measurement_prom())); /**< Display measurement value. */
        }
        else // Measure Not Pass
        {
            lcd_print_string("NOT PASS");                        /**< Indicate measurement did not pass. */
            lcd_print_string(float_to_string(measurement_prom)); /**< Display the failed measurement. */
        }
    }
}

void measure(void)
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

void upt_pid(void)
{
    set = pot_get_value() * MAX_RPM / 100;
    pid_setpoint(set);                  /**< Updates setpoint based on potentiometer input. */
    speed = speedometer_getRPM();       /**< Retrieves the current speed in RPM. */
    motor_set_power(pid_update(speed)); /**< Updates motor power based on PID output with current speed. */
}

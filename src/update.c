#include "update.h"

float set;                /**< Target setpoint value for the motor speed. */
float filtered_speed = 0; /**< Current filtered speed in RPM. */

void systick_init(void)
{
    systick_set_reload(TICKS_FOR_50MS - 1);         /**< Set reload value for 50 ms (3,600,000 ticks at 72 MHz) */
    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB); /**< Use the AHB clock as the SysTick source */
    systick_counter_enable();                       /**< Enable the SysTick counter */
    systick_interrupt_enable();                     /**< Enable SysTick interrupt */
}

void sys_tick_handler(void)
{
    lcd_clear(); /**< Clear the LCD display for fresh output. */

    set = pot_get_value() * MAX_RPM / 100; /**< Calculate the setpoint based on potentiometer value and max RPM. */
    pid_setpoint(set);                     /**< Update the PID controller with the new setpoint. */
    filtered_speed = speedometer_getRPM();       /**< Retrieve the current RPM from the speedometer. */
    motor_set_power(pid_update(filtered_speed)); /**< Update motor power based on PID output with current speed. */

    lcd_print_string("speed:");                        /**< Display the label "speed:" on the LCD. */
    lcd_print_string(float_to_string(filtered_speed)); /**< Display the current speed in RPM. */

    lcd_set_cursor(2, 0);                   /**< Move cursor to the second row for setpoint display. */
    lcd_print_string("SP:");                /**< Display the label "SP:" for setpoint on the LCD. */
    lcd_print_string(float_to_string(set)); /**< Display the current setpoint in RPM. */

    systick_get_countflag(); /**< Check if the SysTick timer has reached zero to clear interrupt flag */
}

#include "pid.h"

PID_Controller pid;
float prev_error; /**< Previous error (for derivative calculation) */
float integral;   /**< Accumulated integral */

void pid_init(PID_Controller* control)
{
    pid = *control; /** Copy the control parameters into the local PID controller */
}

uint8_t pid_update(float measured_value)
{
    /** Calculate the current error as the difference between setpoint and measured value */
    float error = pid.setpoint - measured_value;

    /** Update the integral term by adding the current error */
    integral += error;

    /** Cap the integral to prevent windup */
    if (integral > MAX_INTEGRAL_ERROR)
    {
        integral = MAX_INTEGRAL_ERROR; /** Limit the integral to a maximum threshold */
    }
    else if (integral < -MAX_INTEGRAL_ERROR)
    {
        integral = -MAX_INTEGRAL_ERROR; /** Limit the integral to a minimum threshold */
    }

    /** Calculate the derivative term as the difference from the previous error */
    float derivative = error - prev_error;

    /** Calculate the PID output based on the error, integral, and derivative terms */
    float output = (pid.kp * error) + (pid.ki * integral) + (pid.kd * derivative);

    /** Constrain the output to ensure it stays within allowed limits */
    if (output > MAX_PID_OUTPUT)
    {
        output = MAX_PID_OUTPUT; /** Cap output at the maximum limit */
    }
    else if (output < MIN_PID_OUTPUT)
    {
        output = MIN_PID_OUTPUT; /** Cap output at the minimum limit */
    }

    /** Save the current error to use in the next cycle for the derivative calculation */
    prev_error = error;

    /** Return the constrained output as an 8-bit integer */
    return (uint8_t)output;
}

void pid_setpoint(float setpoint)
{
    pid.setpoint = setpoint; /** Update the setpoint in the PID controller */
}

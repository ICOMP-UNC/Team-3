#include "../include/pid.h"

PID_Controller pid;
static float prev_error; /**< Previous error (for derivative calculation) */
static float integral;   /**< Accumulated integral */

void pid_init(PID_Controller* control)
{
    pid = *control;
}

uint8_t pid_update(float measured_value)
{
    // Calculate error
    float error = pid.setpoint - measured_value;

    // Calculate integral
    integral += error;

    if (integral > MAX_INTEGRAL_ERROR)
    {
        integral = MAX_INTEGRAL_ERROR; // Cap integral to prevent windup
    }
    else if (integral < -MAX_INTEGRAL_ERROR)
    {
        integral = -MAX_INTEGRAL_ERROR;
    }

    // Calculate derivative
    float derivative = error - prev_error;

    // Calculate output
    float output = (pid.kp * error) + (pid.ki * integral) + (pid.kd * derivative);

    // Constrain output
    if (output > MAX_PID_OUTPUT)
    {
        output = MAX_PID_OUTPUT;
    }
    else if (output < MIN_PID_OUTPUT)
    {
        output = MIN_PID_OUTPUT;
    }

    // Save error for next derivative calculation
    prev_error = error;

    return (uint8_t)output;
}

void pid_setpoint(float setpoint)
{
    pid.setpoint = setpoint;
}

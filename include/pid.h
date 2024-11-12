/**
 * @file pid.h
 * @brief Proportional-Integral-Derivative (PID) controller header file.
 *
 * This file defines the structure and functions necessary to initialize and
 * control a PID controller. The PID controller computes an output based on
 * the proportional, integral, and derivative terms derived from the error
 * between a desired setpoint and a measured process variable.
 */

#include <stdint.h>

/**
 * @brief Maximum output value for the PID controller.
 *
 * Sets the upper bound for the controller’s output, typically representing
 * 100% power or the maximum allowable effect of the controller on the system.
 */
#define MAX_PID_OUTPUT 100

/**
 * @brief Minimum output value for the PID controller.
 *
 * Defines the lower bound for the controller's output, usually representing
 * 0% power or the minimum effect of the controller.
 */
#define MIN_PID_OUTPUT 0

/**
 * @brief Maximum allowable accumulated error for the integral term to prevent windup.
 *
 * Caps the accumulated error in the integral term to prevent excessive overshoot,
 * a condition known as integral windup.
 */
#define MAX_INTEGRAL_ERROR 30000.0

/**
 * @struct PID_Controller
 * @brief Structure that holds the PID controller parameters and setpoint.
 *
 * This structure defines the gains for the proportional, integral,
 * and derivative terms, as well as the target setpoint for the control loop.
 */
typedef struct
{
    float kp;       /**< Proportional gain. */
    float ki;       /**< Integral gain. */
    float kd;       /**< Derivative gain. */
    float setpoint; /**< Desired setpoint for the controller. */
} PID_Controller;

/**
 * @brief Initializes the PID controller with specified gains and desired reference.
 *
 * This function sets up the PID controller structure with given proportional,
 * integral, and derivative gains, as well as the initial setpoint for the control loop.
 *
 * @param pid Pointer to the PID controller structure.
 */
void pid_init(PID_Controller* pid);

/**
 * @brief Updates the PID controller and calculates the control output.
 *
 * This function computes the PID control output based on the current error
 * between the setpoint and the measured value. It updates the integral and
 * derivative terms, calculates the control action, and constrains it to
 * the predefined limits.
 *
 * @param measured_value The current measured value of the process variable.
 * @return The control output, constrained between MIN_PID_OUTPUT and MAX_PID_OUTPUT.
 */
uint8_t pid_update(float measured_value);

/**
 * @brief Sets the desired setpoint for the PID controller.
 *
 * This function allows updating the setpoint of the PID controller,
 * defining the target value that the system should aim to reach.
 *
 * @param setpoint The desired setpoint for the control loop.
 */
void pid_setpoint(float setpoint);

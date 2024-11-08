#include <stdint.h>

#define MAX_PID_OUTPUT 100
#define MIN_PID_OUTPUT 0
#define MAX_INTEGRAL_ERROR 100.0

typedef struct
{
    float kp;       /**< Proportional gain */
    float ki;       /**< Integral gain */
    float kd;       /**< Derivative gain */
    float setpoint; /**< Desired setpoint */
} PID_Controller;

/**
 * @brief Initializes the PID controller with gains and desired reference.
 *
 * @param pid Pointer to the PID controller structure.
 */
void pid_init(PID_Controller *pid);

/**
 * @brief Updates the PID controller and calculates the control output.
 *
 * @param pid Pointer to the PID controller structure.
 * @param measured_value The current measured value of the process variable.
 * @return The control output from (0 - 100)%.
 */
uint8_t pid_update(PID_Controller *pid, float measured_value);

/**
 * @brief Sets the desired setpoint for the PID controller.
 *
 * @param pid Pointer to the PID controller structure.
 * @param setpoint The desired setpoint.
 */
void pid_setpoint(PID_Controller *pid, float setpoint);
#include "libopencm3/cm3/nvic.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/usart.h"
#include "pid.h"

/**
 * @brief Initializes the UART peripheral for communication.
 *
 * This function configures the GPIO pins for UART (PA9 for TX and PA10 for RX),
 * sets up USART1 with the desired settings (9600 baud rate, 8 data bits, 1 stop bit),
 * and enables interrupts for receiving data.
 */
void uart_init(void);

/**
 * @brief Interrupt Service Routine for USART1.
 *
 * This function handles received data on USART1. It stores characters in a buffer
 * until a newline or carriage return is detected, at which point it processes the command.
 */
void usart1_isr(void);

/**
 * @brief Processes a command received via UART.
 *
 * This function interprets commands in the format "SET PARAM VALUE" and updates
 * the corresponding PID parameter or setpoint.
 *
 * @param command Pointer to the received command string.
 */
void process_uart_command(const char* command);

/**
 * @brief Sends a null-terminated string via UART.
 *
 * This function transmits a string character by character using the blocking method.
 *
 * @param str Pointer to the string to send.
 */
void uart_send_string(const char* str);

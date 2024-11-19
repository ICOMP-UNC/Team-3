#include "uart.h"

volatile char uart_rx_buffer[64]; /**< Buffer to store received data from UART. */

volatile uint8_t uart_rx_index = 0; /**< Index to keep track of the current position in the receive buffer. */

extern PID_Controller c; /**< External reference to the PID controller instance. */

void uart_init(void)
{
    /** Enable clocks for GPIOA and USART1 */
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_USART1);

    /** Configure PA9 (TX) as alternate function push-pull output with high speed */
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO9);

    /** Configure PA10 (RX) as floating input */
    gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO10);

    /** Configure USART1 parameters */
    usart_set_baudrate(USART1, 9600);                       // Set baud rate to 9600
    usart_set_databits(USART1, 8);                          // Use 8 data bits
    usart_set_stopbits(USART1, USART_STOPBITS_1);           // Set 1 stop bit
    usart_set_mode(USART1, USART_MODE_TX_RX);               // Enable both TX and RX modes
    usart_set_parity(USART1, USART_PARITY_NONE);            // No parity check
    usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE); // No flow control

    /** Enable RX interrupt */
    usart_enable_rx_interrupt(USART1);

    /** Configure NVIC to handle USART1 interrupts */
    nvic_enable_irq(NVIC_USART1_IRQ);

    /** Enable USART1 */
    usart_enable(USART1);
}

void usart1_isr(void)
{
    /** Check if the RXNE flag is set, indicating data is available to read */
    if (usart_get_flag(USART1, USART_SR_RXNE))
    {
        /** Read the received character */
        char received_char = usart_recv(USART1);

        /** Store the character in the buffer if space is available */
        if (uart_rx_index < sizeof(uart_rx_buffer) - 1)
        {
            uart_rx_buffer[uart_rx_index++] = received_char;

            /** If a newline or carriage return is received, process the command */
            if (received_char == '\n' || received_char == '\r')
            {
                uart_rx_buffer[uart_rx_index] = '\0'; // Null-terminate the string
                uart_rx_index = 0;                    // Reset the index
                process_uart_command(uart_rx_buffer); // Process the received command
            }
        }
        else
        {
            /** If the buffer is full, reset the index to prevent overflow */
            uart_rx_index = 0;
        }
    }
}

void process_uart_command(const char* command)
{
    float value;
    char param[16];

    /** Parse the command into a parameter name and value */
    if (sscanf(command, "SET %s %f", param, &value) == 2)
    {
        if (strcmp(param, "KP") == 0)
        {
            c.kp = value; // Update proportional gain
            uart_send_string("KP updated successfully.\n");
        }
        else if (strcmp(param, "KI") == 0)
        {
            c.ki = value; // Update integral gain
            uart_send_string("KI updated successfully.\n");
        }
        else if (strcmp(param, "KD") == 0)
        {
            c.kd = value; // Update derivative gain
            uart_send_string("KD updated successfully.\n");
        }
        else if (strcmp(param, "SP") == 0)
        {
            c.setpoint = value; // Update setpoint
            uart_send_string("Setpoint updated successfully.\n");
        }
        else
        {
            /** Handle invalid parameter names */
            uart_send_string("Invalid parameter.\n");
        }
    }
    else
    {
        /** Handle invalid command format */
        uart_send_string("Invalid command format. Use 'SET PARAM VALUE'.\n");
    }
}

void uart_send_string(const char* str)
{
    while (*str)
    {
        usart_send_blocking(USART1, *str++);
    }
}

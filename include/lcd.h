/**
 * @file lcd.h
 * @brief LCD driver using STM32 microcontroller and I2C expander PCF8574.
 *
 * This file contains functions to initialize and control a 16x2 LCD
 * via I2C protocol, using an STM32 microcontroller and a PCF8574 as an I2C expander.
 */

#include "libopencm3/stm32/gpio.h"
#include "libopencm3/stm32/i2c.h"
#include "libopencm3/stm32/rcc.h"

/**
 * @brief I2C address of the PCF8574.
 *
 * The address used by the I2C expander to communicate with the LCD.
 */
#define PCF8574_ADDRESS 0x27

/**
 * @brief STM32 SDA pin.
 *
 * The SDA (data) pin used for I2C communication.
 */
#define SDA_PIN GPIO7

/**
 * @brief STM32 SCL pin.
 *
 * The SCL (clock) pin used for I2C communication.
 */
#define SCL_PIN GPIO6

// LCD Commands
/**
 * @brief Command to clear the LCD screen.
 */
#define LCD_CLEARDISPLAY 0x01

/**
 * @brief Command to return the cursor to the home position.
 */
#define LCD_RETURNHOME 0x02

/**
 * @brief Command to set the LCD entry mode.
 */
#define LCD_ENTRYMODESET 0x04

/**
 * @brief Command to control the display settings.
 */
#define LCD_DISPLAYCONTROL 0x08

/**
 * @brief Command to set LCD function parameters.
 */
#define LCD_FUNCTIONSET 0x20

/**
 * @brief Command to set DDRAM address.
 */
#define LCD_SETDDRAMADDR 0x80

// Flags for display entry mode
/**
 * @brief Flag for left-to-right text entry mode.
 */
#define LCD_ENTRYLEFT 0x02

// Flags for display control
/**
 * @brief Flag to turn the display on.
 */
#define LCD_DISPLAYON 0x04

/**
 * @brief Flag to turn the cursor off.
 */
#define LCD_CURSOROFF 0x00

/**
 * @brief Flag to disable cursor blinking.
 */
#define LCD_BLINKOFF 0x00

// Flags for function set
/**
 * @brief Flag to set 4-bit data mode.
 */
#define LCD_4BITMODE 0x00

/**
 * @brief Flag to set 2-line display mode.
 */
#define LCD_2LINE 0x08

/**
 * @brief Flag to set 5x8 character dot size.
 */
#define LCD_5x8DOTS 0x00

// Flags for backlight control
/**
 * @brief Flag to turn the LCD backlight on.
 */
#define LCD_BACKLIGHT 0x08

/**
 * @brief Flag to turn the LCD backlight off.
 */
#define LCD_NOBACKLIGHT 0x00

/**
 * @brief Enable bit for LCD command latching.
 */
#define LCD_EN 0B00000100

/**
 * @brief Register select bit for choosing between command and data mode.
 */
#define LCD_RS 0B00000001

/**
 * @brief Conversion factor to create a delay in milliseconds.
 *
 * Used in delay functions to calculate the number of ticks for a given
 * millisecond delay.
 */
#define TICKS_TO_MS 8000

/**
 * @brief Initializes the LCD, pins, and I2C communication.
 *
 * Configures the LCD for 4-bit mode, 2-line display, and enables the backlight if required.
 * Sets up the peripheral clock, I2C settings, and other initial configurations.
 */
void lcd_init(void);

/**
 * @brief Sends a pulse to enable the LCD.
 *
 * Sets the enable bit (EN) to latch the data or command currently being sent to the LCD.
 *
 * @param data Data to send along with the enable pulse.
 */
void lcd_pulse_enable(uint8_t data);

/**
 * @brief Sends a nibble (4 bits) of data or command to the LCD.
 *
 * Sends the higher nibble of a byte in 4-bit mode, typically used for initialization or sending data.
 *
 * @param nibble 4-bit data to send.
 * @param mode Mode for the data (0 for command, 1 for data).
 */
void lcd_send_nibble(uint8_t nibble, uint8_t mode);

/**
 * @brief Sends a full byte of data or command to the LCD.
 *
 * Sends both nibbles of a byte to the LCD in 4-bit mode, either as data or a command.
 *
 * @param byte 8-bit data to send.
 * @param mode Mode for the data (0 for command, 1 for data).
 */
void lcd_send_byte(uint8_t byte, uint8_t mode);

/**
 * @brief Prints a single character on the LCD.
 *
 * Sends a character to be displayed at the current cursor position.
 *
 * @param c Character to print.
 */
void lcd_print_char(char c);

/**
 * @brief Prints a string on the LCD.
 *
 * Sends a null-terminated string to the LCD, displaying it from the current cursor position.
 *
 * @param str Pointer to the null-terminated string to print.
 */
void lcd_print_string(const char* str);

/**
 * @brief Clears the LCD display.
 *
 * Clears any text on the LCD screen and resets the cursor position to home.
 */
void lcd_clear(void);

/**
 * @brief Sets the cursor to a specific position on the LCD.
 *
 * Positions the cursor to a specified row and column on a 16x2 LCD.
 *
 * @param row The row number (0 for the first row, 1 for the second row).
 * @param col The column number (0 to 15 for a 16x2 LCD).
 */
void lcd_set_cursor(uint8_t row, uint8_t col);

/**
 * @brief Creates a delay for a specified time in milliseconds.
 *
 * Introduces a delay by performing a software loop, useful for timing-sensitive operations on the LCD.
 *
 * @param ms Time to wait in milliseconds.
 */
void delay_ms(uint32_t ms);

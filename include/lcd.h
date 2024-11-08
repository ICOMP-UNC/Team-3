/**
 * @file lcd_driver.c
 * @brief LCD driver using STM32 microcontroller and I2C expander PCF8574.
 *
 * This file contains functions to initialize and control a 16x2 LCD
 * via I2C protocol, using an STM32 microcontroller and a PCF8574 as an I2C expander.
 */

#include "libopencm3/stm32/gpio.h"
#include "libopencm3/stm32/i2c.h"
#include "libopencm3/stm32/rcc.h"

#define PCF8574_ADDRESS 0x27  ///< I2C address of the PCF8574.
#define SDA_PIN         GPIO7 ///< STM32 SDA pin.
#define SCL_PIN         GPIO6 ///< STM32 SCL pin.

// LCD Commands
#define LCD_CLEARDISPLAY   0x01 ///< Command to clear the LCD screen.
#define LCD_RETURNHOME     0x02 ///< Command to return the cursor to the home position.
#define LCD_ENTRYMODESET   0x04 ///< Command to set the LCD entry mode.
#define LCD_DISPLAYCONTROL 0x08 ///< Command to control the display settings.
#define LCD_FUNCTIONSET    0x20 ///< Command to set LCD function parameters.
#define LCD_SETDDRAMADDR   0x80 ///< Command to set DDRAM address.

// Flags for display entry mode
#define LCD_ENTRYLEFT 0x02 ///< Flag for left-to-right text entry mode.

// Flags for display control
#define LCD_DISPLAYON 0x04 ///< Flag to turn the display on.
#define LCD_CURSOROFF 0x00 ///< Flag to turn the cursor off.
#define LCD_BLINKOFF  0x00 ///< Flag to disable cursor blinking.

// Flags for function set
#define LCD_4BITMODE 0x00 ///< Flag to set 4-bit data mode.
#define LCD_2LINE    0x08 ///< Flag to set 2-line display mode.
#define LCD_5x8DOTS  0x00 ///< Flag to set 5x8 character dot size.

// Flags for backlight control
#define LCD_BACKLIGHT   0x08 ///< Flag to turn the LCD backlight on.
#define LCD_NOBACKLIGHT 0x00 ///< Flag to turn the LCD backlight off.

#define LCD_EN 0B00000100 ///< Enable bit.
#define LCD_RS 0B00000001 ///< Register select bit.

/**
 * @brief Initializes the LCD, Pins and I2C1 comunication
 *
 * This function configures the LCD for 4-bit mode, 2-line display,
 * and enables the backlight if required, also sets the peripheral clock,
 * i2c setup and wherever it needs to work as normal.
 */
void lcd_init(void);

/**
 * @brief Sends a pulse to enable the LCD.
 *
 * @param data Data to send along with the enable pulse.
 */
void lcd_pulse_enable(uint8_t data);

/**
 * @brief Sends a nibble (4 bits) of data or command to the LCD.
 *
 * @param nibble 4-bit data to send.
 * @param mode Mode for the data (0 for command, 1 for data).
 */
void lcd_send_nibble(uint8_t nibble, uint8_t mode);

/**
 * @brief Sends a full byte of data or command to the LCD.
 *
 * @param byte 8-bit data to send.
 * @param mode Mode for the data (0 for command, 1 for data).
 */
void lcd_send_byte(uint8_t byte, uint8_t mode);

/**
 * @brief Prints a single character on the LCD.
 *
 * @param c Character to print.
 */
void lcd_print_char(char c);

/**
 * @brief Prints a string on the LCD.
 *
 * @param str Pointer to the null-terminated string to print.
 */
void lcd_print_string(const char* str);

/**
 * @brief Clears any display data
 *
 * This function clears anything appearing on the screen and
 * sets the cursor position to home.
 */
void lcd_clear(void);

/**
 * @brief Sets the cursor to a specific position on the LCD.
 *
 * This function sets the cursor to a specific row and column on the LCD.
 *
 * @param row The row number (0 for the first row, 1 for the second row).
 * @param col The column number (0 to 15 for a 16x2 LCD).
 */
void lcd_set_cursor(uint8_t row, uint8_t col);

/**
 * @brief does a delay.
 *
 * @param ms Time to wait in [ms].d
 */
void delay_ms(uint32_t ms);
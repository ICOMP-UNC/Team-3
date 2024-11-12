#include "lcd.h"

/** Set the EN line high to latch the command, then set it low */
void lcd_pulse_enable(uint8_t data)
{
    uint8_t d = data | LCD_EN;                            /** Set the EN bit high */
    i2c_transfer7(I2C1, PCF8574_ADDRESS, &d, 1, NULL, 0); /** Send data with EN high to latch the command */
    delay_ms(1);                                          /** Small delay for stability */

    d = data & ~LCD_EN;                                   /** Clear the EN bit */
    i2c_transfer7(I2C1, PCF8574_ADDRESS, &d, 1, NULL, 0); /** Send data with EN low to complete the pulse */
    delay_ms(1);                                          /** Small delay to allow the LCD to process */
}

// Send a nibble to the LCD via PCF8574
void lcd_send_nibble(uint8_t nibble, uint8_t mode)
{
    uint8_t data = (nibble & 0xF0) | mode |
                   LCD_BACKLIGHT; /** Prepare the data: isolate the upper nibble, add mode and backlight */
    lcd_pulse_enable(data);       /** Send the nibble via pulse */
}

// Send a command or data byte to the LCD
void lcd_send_byte(uint8_t byte, uint8_t mode)
{
    lcd_send_nibble(byte & 0xF0, mode);        /** Send the upper nibble */
    lcd_send_nibble((byte << 4) & 0xF0, mode); /** Send the lower nibble */
    delay_ms(2);                               /** Delay for LCD processing after sending the byte */
}

/** Initialize the LCD by setting up the I2C, configuring the LCD mode, and clearing the display */
void lcd_init(void)
{
    rcc_periph_clock_enable(RCC_GPIOB); /** Enable GPIOB clock for I2C */
    rcc_periph_clock_enable(RCC_I2C1);  /** Enable I2C1 clock */

    gpio_set_mode(GPIOB,
                  GPIO_MODE_OUTPUT_50_MHZ,
                  GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
                  SDA_PIN | SCL_PIN); /** Set GPIOB SDA and SCL pins to I2C mode */

    i2c_peripheral_disable(I2C1);      /** Disable I2C1 to configure it */
    i2c_set_standard_mode(I2C1);       /** Set I2C to standard mode (100kHz) */
    i2c_set_clock_frequency(I2C1, 36); /** Set I2C clock frequency to 36 MHz */
    i2c_set_trise(I2C1, 36);           /** Set rise time */
    i2c_set_ccr(I2C1, 180);            /** Set I2C clock control register for 180kHz */
    i2c_peripheral_enable(I2C1);       /** Enable I2C1 */

    delay_ms(50); /** Wait for LCD to power up */

    lcd_send_nibble(0x30, 0); /** Send Function set command in 8-bit mode */
    delay_ms(5);              /** Delay for LCD to process */
    lcd_send_nibble(0x30, 0); /** Repeat 8-bit function set */
    delay_ms(5);              /** Delay for LCD to process */
    lcd_send_nibble(0x30, 0); /** Repeat once more */
    delay_ms(5);              /** Delay for LCD to process */
    lcd_send_nibble(0x20, 0); /** Switch to 4-bit mode */
    delay_ms(5);              /** Delay for LCD to process */

    /** Configure LCD settings in 4-bit mode */
    lcd_send_byte(LCD_FUNCTIONSET | LCD_2LINE | LCD_5x8DOTS | LCD_4BITMODE, 0);          /** 4-bit, 2-line display */
    lcd_send_byte(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF, 0); /** Display on, cursor off */
    lcd_send_byte(LCD_ENTRYMODESET | LCD_ENTRYLEFT, 0); /** Set entry mode: cursor moves left */
    lcd_send_byte(LCD_CLEARDISPLAY, 0);                 /** Clear the display */
    delay_ms(2);                                        /** Delay for display clear */
}

// Print a single character
void lcd_print_char(char c)
{
    lcd_send_byte(c, LCD_RS);
}

/** Send a null-terminated string to the LCD */
void lcd_print_string(const char* str)
{
    while (*str) /** Loop through the string until null character */
    {
        lcd_print_char(*str++);
    }
}

/** Clear the LCD display */
void lcd_clear(void)
{
    lcd_send_byte(LCD_CLEARDISPLAY, 0); /** Send clear display command */
    delay_ms(1);                        /** Delay to allow LCD to process clear command */
}

void lcd_set_cursor(uint8_t row, uint8_t col)
{
    uint8_t address;

    if (row == 0) /** Determine address for row 0 */
    {
        address = 0x00 + col;
    }
    else /** Determine address for row 1 */
    {
        address = 0x40 + col;
    }

    lcd_send_byte(LCD_SETDDRAMADDR | address, 0); /** Set DDRAM address to position cursor */
}

/** Create a delay in milliseconds */
void delay_ms(uint32_t ms)
{
    for (uint32_t i = 0; i < ms * TICKS_TO_MS; i++)
    {
        __asm__("nop"); /** No-operation loop to create delay */
    }
}

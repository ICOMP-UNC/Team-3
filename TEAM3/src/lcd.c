#include "../include/LCD.h"

// Function to pulse the EN line to latch commands
void lcd_pulse_enable(uint8_t data)
{
    uint8_t d = data | LCD_EN;
    i2c_transfer7(I2C1, PCF8574_ADDRESS, &d, 1, NULL, 0);
    delay_ms(1);
    d = data & ~LCD_EN;
    i2c_transfer7(I2C1, PCF8574_ADDRESS, &d, 1, NULL, 0);
    delay_ms(1);
}

// Send a nibble to the LCD via PCF8574
void lcd_send_nibble(uint8_t nibble, uint8_t mode)
{
    uint8_t data = (nibble & 0xF0) | mode | LCD_BACKLIGHT;
    lcd_pulse_enable(data);
}

// Send a command or data byte to the LCD
void lcd_send_byte(uint8_t byte, uint8_t mode)
{
    lcd_send_nibble(byte & 0xF0, mode);
    lcd_send_nibble((byte << 4) & 0xF0, mode);
    delay_ms(2); // Delay to allow the LCD to process the byte
}

// LCD initialization function with adjusted timings
void lcd_init(void)
{
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_I2C1);

    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, SDA_PIN | SCL_PIN);

    i2c_peripheral_disable(I2C1);
    i2c_set_standard_mode(I2C1);
    i2c_set_clock_frequency(I2C1, I2C_CR2_FREQ_36MHZ);
    i2c_set_trise(I2C1, 36);
    i2c_set_ccr(I2C1, 180);
    i2c_peripheral_enable(I2C1);

    delay_ms(50); // Wait for LCD to power up

    lcd_send_nibble(0x30, 0); // Function set: 8-bit mode
    delay_ms(5);
    lcd_send_nibble(0x30, 0); // Repeat
    delay_ms(5);
    lcd_send_nibble(0x30, 0); // Repeat again
    delay_ms(5);
    lcd_send_nibble(0x20, 0); // Set to 4-bit mode
    delay_ms(5);

    // Configure LCD in 4-bit mode
    lcd_send_byte(LCD_FUNCTIONSET | LCD_2LINE | LCD_5x8DOTS | LCD_4BITMODE, 0);          // 4-bit, 2 lines
    lcd_send_byte(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF, 0); // Display on, cursor off
    lcd_send_byte(LCD_ENTRYMODESET | LCD_ENTRYLEFT, 0);                                  // Entry mode set
    lcd_send_byte(LCD_CLEARDISPLAY, 0);                                                  // Clear display
    delay_ms(2);
}

// Print a single character
void lcd_print_char(char c)
{
    lcd_send_byte(c, LCD_RS);
}

// Print a string
void lcd_print_string(const char *str)
{
    while (*str)
    {
        lcd_print_char(*str++);
    }
}

void lcd_clear(void)
{
    lcd_send_byte(LCD_CLEARDISPLAY, 0);
    delay_ms(1);
}

void lcd_set_cursor(uint8_t row, uint8_t col)
{
    uint8_t address;

    if (row == 0)
    {
        address = 0x00 + col;
    }
    else
    {
        address = 0x40 + col;
    }

    lcd_send_byte(LCD_SETDDRAMADDR | address, 0); // Configura la dirección de DDRAM
}

void delay_ms(uint32_t ms)
{
    for (uint32_t i = 0; i < ms * 8000; i++)
    {
        __asm__("nop");
    }
}
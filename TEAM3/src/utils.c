#include "../include/Utils.h"

char* float_to_string(float number) {
    static char str[20];  // Static buffer to hold the string (must be large enough)
    int integer_part = (int)number;
    int decimal_part = (int)((number - integer_part) * 100);  // 2 decimal places

    // Handle integer part
    int i = 0;
    if (integer_part == 0) {
        str[i++] = '0';  // Handle 0 case
    } else {
        if (integer_part < 0) {
            str[i++] = '-';  // Negative numbers
            integer_part = -integer_part;
        }

        // Convert integer part to string
        int num = integer_part;
        int reverse_int = 0;
        while (num != 0) {
            reverse_int = reverse_int * 10 + num % 10;
            num /= 10;
        }

        while (reverse_int != 0) {
            str[i++] = reverse_int % 10 + '0';
            reverse_int /= 10;
        }
    }

    str[i++] = '.';  // Add decimal point

    // Handle decimal part
    if (decimal_part == 0) {
        str[i++] = '0';  // If no decimal value, just add 0
        str[i++] = '0';  // Two decimal places
    } else {
        if (decimal_part < 0) {
            decimal_part = -decimal_part;  // For negative values
        }

        // Convert decimal part to string
        str[i++] = (decimal_part / 10) + '0';
        str[i++] = (decimal_part % 10) + '0';
    }

    str[i] = '\0';  // Null-terminate the string
    return str;
}
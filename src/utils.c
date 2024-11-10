#include "../include/Utils.h"

char* float_to_string(float number)
{
    static char str[20]; // Static buffer to hold the string (must be large enough)
    int integer_part = (int)number;
    int decimal_part = (int)((number - integer_part) * 100); // 2 decimal places

    // Handle integer part
    int i = 0;
    if (integer_part == 0)
    {
        str[i++] = '0'; // Handle 0 case
    }
    else
    {
        if (integer_part < 0)
        {
            str[i++] = '-'; // Negative numbers
            integer_part = -integer_part;
        }

        // Convert integer part to string without reverse trick
        int j = i;
        int num = integer_part;
        // Extract digits from right to left
        char temp[10]; // Temporary buffer to store the digits
        int len = 0;
        while (num != 0)
        {
            temp[len++] = (num % 10) + '0'; // Store digits as characters
            num /= 10;
        }

        // Copy the digits to the str buffer in the correct order
        for (int k = len - 1; k >= 0; k--)
        {
            str[i++] = temp[k];
        }
    }

    // Handle decimal point only if necessary
    if (decimal_part != 0)
    {
        str[i++] = '.'; // Add decimal point

        // Handle decimal part
        if (decimal_part < 0)
        {
            decimal_part = -decimal_part; // For negative values
        }

        // Convert decimal part to string
        str[i++] = (decimal_part / 10) + '0';
        str[i++] = (decimal_part % 10) + '0';
    }

    // Null-terminate the string
    str[i] = '\0';
    return str;
}

#include "../include/Utils.h"

char* float_to_string(float number)
{
    static char str[20];            /** Static buffer to hold the string (must be large enough to hold the output) */
    int integer_part = (int)number; /** Extract the integer part of the float */
    int decimal_part = (int)((number - integer_part) * 100); /** Extract the decimal part (2 decimal places) */

    /** Handle the integer part */
    int i = 0; /** Index for placing characters in `str` */
    if (integer_part == 0)
    {
        str[i++] = '0'; /** Handle case where integer part is 0 */
    }
    else
    {
        if (integer_part < 0)
        {
            str[i++] = '-';               /** Add minus sign for negative numbers */
            integer_part = -integer_part; /** Convert integer part to positive */
        }

        /** Convert the integer part to a string without reversing */
        int num = integer_part;
        char temp[10]; /** Temporary buffer to store the integer digits */
        int len = 0;

        /** Extract digits from right to left */
        while (num != 0)
        {
            temp[len++] = (num % 10) + '0'; /** Store each digit as a character */
            num /= 10;                      /** Remove the last digit from the number */
        }

        /** Copy digits from `temp` to `str` in the correct order */
        for (int k = len - 1; k >= 0; k--)
        {
            str[i++] = temp[k]; /** Place each digit in the main string */
        }
    }

    /** Handle the decimal point only if necessary */
    if (decimal_part != 0)
    {
        str[i++] = '.'; /** Add decimal point */

        /** Handle the decimal part for negative values */
        if (decimal_part < 0)
        {
            decimal_part = -decimal_part; /** Convert to positive */
        }

        /** Convert and add the decimal part digits */
        str[i++] = (decimal_part / 10) + '0'; /** Tens place */
        str[i++] = (decimal_part % 10) + '0'; /** Units place */
    }

    /** Null-terminate the string */
    str[i] = '\0';

    return str; /** Return the pointer to the static buffer */
}

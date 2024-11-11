/**
 * @file utils.h
 * @brief Provides a function to convert a floating-point number to a string.
 *
 * This module defines a utility function for converting floating-point numbers
 * to a string representation. Note that there is a known issue with handling
 * certain multiples of 10, where precision limitations may cause unexpected formatting.
 * This function is useful for displaying or logging float values in embedded systems
 * where standard libraries may lack complete floating-point support.
 */

#include <stdio.h>

/**
 * @brief Converts a floating-point number to a string.
 *
 * This function takes a float as input and converts it to a string
 * representation with a fixed number of decimal places. Due to limitations
 * in floating-point precision, certain multiples of 10 (e.g., 10, 20, 30)
 * may not be converted with complete accuracy.
 *
 * @param number The floating-point number to convert.
 * @return Pointer to a static string buffer representing the number.
 *         The buffer is static and reused on each call.
 */
char* float_to_string(float number);

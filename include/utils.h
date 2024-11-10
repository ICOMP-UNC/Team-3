/**
 * @file float_conversion.h
 * @brief Provides a function to convert a floating-point number to a string.
 *
 * This module defines a utility function for converting floating-point numbers
 * to a string representation. Note that there is a known issue with handling
 * certain multiples of 10, where precision may cause unexpected formatting.
 */

#include <stdio.h>

/**
 * @brief Converts a floating-point number to a string.
 *
 * This function takes a float as input and converts it to a string
 * representation. Note that due to limitations in floating-point precision,
 * some multiples of 10 (e.g., 10, 20, 30) may not be converted accurately.
 *
 * @param number The floating-point number to convert.
 * @return Pointer to a string representing the number.
 */
char* float_to_string(float number);

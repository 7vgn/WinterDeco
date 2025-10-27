/**
 * @file touch.h
 * @date 2024-10-06
 * @brief Driver for CVD-based touch sensors using the ADC
 * Uses Pin RA5 for the sensor, RB5 for Guard A, and RB7 for Guard B. 
 */

#ifndef TOUCH_H
#define	TOUCH_H

#include<stdbool.h>

/**
 * @brief Initialise the driver
 * @details Must be called before any other function in this library.
 */
void touchInit();

/**
 * @brief Determine if the sensor is currently touched or not
 * @param sensor The sensor to be checked.
 * @return Returns true if the sensor is touched, otherwise false.
 */
bool isTouched();

#endif // TOUCH_H

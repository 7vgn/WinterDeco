/**
 * @file touch.h
 * @date 2024-10-06
 * @brief Driver for CVD-based touch sensors using the ADC
 * Uses Pins RA4 (left arm), RA5 (left foot), RB6 (hat), RB5 (right arm),
 * RB4 (right foot) for the sensors, RC7 for Guard A, and RB7 for Guard B. 
 */

#ifndef TOUCH_H
#define	TOUCH_H

#include<stdbool.h>

/**
 * @brief Locations of the Touch Sensors
 *      ----------
 *      |        |
 *      |   HAT  |
 *      |        |
 *    --------------
 *     /          \
 *    /            \
 *    |            |
 *    |            |
 *    \            /
 *    /            \
 *   /              \
 *  /|              |\
 * / | ARM_    ARM_ | \
 * \ | LEFT   RIGHT | /
 *  \|              |/
 *   \              /
 *    \            /
 *    /            \
 *   /              \
 *  /                \
 *  |                |
 *  | FOOT_    FOOT_ |
 *  | LEFT     RIGHT |
 *  |                |
 *  \                /
 *   \              /
 *    \____________/
 */
typedef enum
{
	SENSOR_FOOT_LEFT = 0,
	SENSOR_FOOT_RIGHT,
	SENSOR_ARM_LEFT,
	SENSOR_ARM_RIGHT,
	SENSOR_HAT,
	NUM_SENSORS
} Sensor;

/**
 * @brief Name for each sensor
 */
extern const char* SENSOR_NAMES[NUM_SENSORS];

/**
 * @brief Initialise the driver
 * @details Must be called before any other function in this library.
 */
void touchInit(void);

/**
 * @brief Determine if the sensor is currently touched or not
 * @param sensor The sensor to be checked.
 * @return Returns true if the sensor is touched, otherwise false.
 */
bool isTouched(Sensor sensor);

#endif // TOUCH_H

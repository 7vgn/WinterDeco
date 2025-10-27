/**
 * @file input.h
 * @date 2024-10-06
 * @brief Manages touch sensor input events
 * 
 * This sits on top of the low level touch library. Each time a sensor is
 * pressed or released, an input event is generated. In the case of a sensor
 * release, the duration of the press is also reported. 
 */

#ifndef INPUT_H
#define	INPUT_H

#include<stdbool.h>
#include"touch.h"

/**
 * @brief Enumeration type for touch sensor events
 */
typedef enum
{
	/// Nothing happened
	EVENT_NONE,
	/// The sensor has started being pressed
	EVENT_PRESS,
	/// The sensor has been hold for a "long" time
	EVENT_HOLD_LONG,
	/// The sensor has been released after having been pressed for a short time
	EVENT_RELEASE_SHORT,
	/// The sensor has been released after having been pressed for a long time
	EVENT_RELEASE_LONG
} InputEvent;

/**
 * @brief Defines what constitutes a "long" time
 * @details This is in units of 100ms (or whatever the period between calls to
 * inputUpdate()).
 */
#define LONG_PRESS_DURATION 20

/**
 * @brief Reset the library
 * @details This should be called to initialise the library and when the state
 * of the sensors might have been jumbled (e.g. after waking up from sleep).
 */
void inputReset(void);

/**
 * @brief Queries all touch sensors and calculates input events
 * @details This function is called from the main loop at regular intervals,
 * typically 100ms. Note that inputs shorter than this period might not be
 * detected.
 * @param events Upon return, this array contains the latest input event for
 * each sensor.
 */
void inputUpdate(InputEvent events[NUM_SENSORS]);

/**
 * @brief Get the state of a sensor
 * @details Note that this function does not initiate a measurement, it uses the
 * internally stored state to obtain the information. 
 * @param sensor The sensor in question.
 * @return True if the sensor is currently pressed, false otherwise. 
 */
bool inputPressed(Sensor sensor);

/**
 * @brief Check if any sensor is currently pressed
 * @details Note that this function does not initiate a measurement, it uses the
 * internally stored state to obtain the information. 
 * @return True if any sensor is currently pressed, false otherwise. 
 */
bool inputPressedAny(void);

#endif // INPUT_H

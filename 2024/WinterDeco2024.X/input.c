/**
 * @file input.c
 * @date 2024-10-06
 * @brief Implements input.h
 */

#include<stdbool.h>
#include<stdint.h>
#include<xc.h>
#include"input.h"

/**
 * @brief Current status of the touch sensors (pressed or not)
 */
static bool isPressed[NUM_SENSORS];

/**
 * @brief Press duration for sensors that are currently pressed
 */
static uint8_t pressDuration[NUM_SENSORS];

void inputReset()
{
	for(uint8_t i = 0; i < NUM_SENSORS; i++)
	{
		isPressed[i] = false;
		pressDuration[i] = 0;
	}
}

void inputUpdate(InputEvent events[NUM_SENSORS])
{
	for(uint8_t i = 0; i < NUM_SENSORS; i++)
	{
		// Initialise event
		events[i] = EVENT_NONE;
		
		// Increment press duration
		if(isPressed[i] && pressDuration[i] < 255)
			pressDuration[i]++;
		
		// Get current state of the sensor
		bool touched = isTouched(i);
		
		// Detect events
		if(touched && !isPressed[i])
		{
			// Sensor was just pressed
			isPressed[i] = true;
			pressDuration[i] = 0;
			events[i] = EVENT_PRESS;
		}
		else if(touched && isPressed[i] && pressDuration[i] >= LONG_PRESS_DURATION)
		{
			events[i] = EVENT_HOLD_LONG;
		}
		else if(!touched && isPressed[i])
		{
			// Sensor was just released
			isPressed[i] = false;
			// Check how long it was pressed for
			events[i] = pressDuration[i] >= LONG_PRESS_DURATION ? EVENT_RELEASE_LONG : EVENT_RELEASE_SHORT;
		}
	}
}

bool inputPressed(Sensor sensor)
{
	return isPressed[sensor];
}

bool inputPressedAny()
{
	for(uint8_t i = 0; i < NUM_SENSORS; i++)
		if(isPressed[i])
			return true;
	return false;
}

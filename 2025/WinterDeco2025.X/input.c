/**
 * @file input.c
 * @date 2025-10-21
 * @brief Implements input.h
 */

#include<stdbool.h>
#include<stdint.h>
#include<xc.h>
#include"input.h"

/**
 * @brief Current status of the buttons (pressed or not)
 */
static bool isPressed[NUM_BUTTONS];

/**
 * @brief Press duration for buttons that are currently pressed
 */
static uint8_t pressDuration[NUM_BUTTONS];

void inputInit()
{
	// Configure pins as digital inputs with weak pull-up
	TRISBbits.TRISB4 = 1;
	TRISBbits.TRISB5 = 1;
	TRISBbits.TRISB6 = 1;
	ANSELBbits.ANSELB4 = 0;
	ANSELBbits.ANSELB5 = 0;
	ANSELBbits.ANSELB6 = 0;
	WPUBbits.WPUB4 = 1;
	WPUBbits.WPUB5 = 1;
	WPUBbits.WPUB6 = 1;
	
	// Reset internal state
	for(uint8_t i = 0; i < NUM_BUTTONS; i++)
	{
		isPressed[i] = false;
		pressDuration[i] = 0;
	}
}

void inputUpdate(InputEvent events[NUM_BUTTONS])
{
	for(uint8_t i = 0; i < NUM_BUTTONS; i++)
	{
		// Initialise event
		events[i] = EVENT_NONE;
		
		// Increment press duration
		if(isPressed[i] && pressDuration[i] < 255)
			pressDuration[i]++;
		
		// Get current state of the button
		bool pressed = ((PORTB >> (4 + i)) & 1) == 0;
		
		// Detect events
		if(pressed && !isPressed[i])
		{
			// Button was just pressed
			isPressed[i] = true;
			pressDuration[i] = 0;
			events[i] = EVENT_PRESS;
		}
		else if(pressed && isPressed[i] && pressDuration[i] >= LONG_PRESS_DURATION)
		{
			events[i] = EVENT_HOLD_LONG;
		}
		else if(!pressed && isPressed[i])
		{
			// Button was just released
			isPressed[i] = false;
			// Check how long it was pressed for
			events[i] = pressDuration[i] >= LONG_PRESS_DURATION ? EVENT_RELEASE_LONG : EVENT_RELEASE_SHORT;
		}
	}
}

bool inputPressed(Button button)
{
	return isPressed[button];
}

bool inputPressedAny()
{
	for(uint8_t i = 0; i < NUM_BUTTONS; i++)
		if(isPressed[i])
			return true;
	return false;
}

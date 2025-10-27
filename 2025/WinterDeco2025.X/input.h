/**
 * @file input.h
 * @date 2025-10-21
 * @brief Manages input events
 * 
 * Each time a button is pressed or released, an input event is generated. In
 * the case of a button release, the duration of the press is also calculated. 
 */

#ifndef INPUT_H
#define	INPUT_H

#include<stdbool.h>

/**
 * @brief Enumeration type for buttons
 */
typedef enum
{
	BTN_LEFT, BTN_CENTER, BTN_RIGHT,
	NUM_BUTTONS
} Button;

/**
 * @brief Enumeration type for input events
 */
typedef enum
{
	/// Nothing happened
	EVENT_NONE,
	/// The button has started being pressed
	EVENT_PRESS,
	/// The button has been held for a "long" time
	EVENT_HOLD_LONG,
	/// The button has been released after having been pressed for a short time
	EVENT_RELEASE_SHORT,
	/// The button has been released after having been pressed for a long time
	EVENT_RELEASE_LONG
} InputEvent;

/**
 * @brief Defines swhat constitutes a "long" time
 * @details This is in units of 100ms (or whatever the period between calls to
 * inputUpdate()).
 */
#define LONG_PRESS_DURATION 20

/**
 * @brief Initialise the library
 * @details This should be called before any other functions in this library.
 */
void inputInit(void);

/**
 * @brief Queries all buttons and calculates input events
 * @details This function is called from the main loop at regular intervals,
 * typically 100ms. Note that inputs shorter than this period might not be
 * detected.
 * @param events Upon return, this array contains the latest input event for
 * each button.
 */
void inputUpdate(InputEvent events[NUM_BUTTONS]);

/**
 * @brief Get the state of a button
 * @details Note that this function does not query the button itself but instead
 * uses the internally stored state to obtain the information. 
 * @param button The button in question.
 * @return True if the button is currently pressed, false otherwise. 
 */
bool inputPressed(Button button);

/**
 * @brief Check if any button is currently pressed
 * @details Note that this function does not query the button itself but instead
 * uses the internally stored state to obtain the information. 
 * @return True if any button is currently pressed, false otherwise. 
 */
bool inputPressedAny(void);

#endif // INPUT_H

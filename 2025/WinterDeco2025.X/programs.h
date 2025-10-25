/**
 * @file programs.h
 * @date 2025-10-21
 * @brief Programs for creating different light patterns
 */

#ifndef PROGRAMS_H
#define	PROGRAMS_H

#include"input.h"

/**
 * @brief Data structure for programs
 */
typedef struct
{
	/// Name of the program
    char name[32];
	/// Initialisation function called at the start of a program
	/// If no initialisation is needed, this can be null.
	void (*initFunction)(void);
	/// Update function called at 10ms intervals
	/// If no updating is needed, this can be null. 
	/// First parameter is the system clock. 
	/// Second parameter are the input events. A program may process and clear
	/// them (by assigning EVENT_NONE) or ignore them in which case the main
	/// function might process them. 
    void (*updateFunction)(uint16_t, InputEvent[NUM_BUTTONS]);
} Program;


/**
 * @brief Number of implemented programs
 */
extern const uint8_t NUM_PROGRAMS;

/**
 * @brief Array containing all implemented programs
 */
extern const Program PROGRAMS[];

#endif // PROGRAMS_H

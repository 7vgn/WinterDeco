/**
 * @file led.h
 * @date 2025-10-21
 * @brief Driver for multiplexed LEDs
 * 
 * This driver controls the 64 LEDs in a 16x4 matrix configuration (16 rows
 * controlled via the 74HC154 demultiplexer, 4 columns). The rows are on RC[4:7]
 * and the columns on RC[0:3]. RB7 is the demux enable pin (active low).
 * Timer 0 is used for timing the multiplexing. 
 */

#ifndef LED_H
#define	LED_H

#include<stdint.h>

/* 
 * Positions of the LEDs:
 * 
 *   C0 C1 C2 C3 C0 C1 C2 C3  
 * ├────────────┼────────────┤
 * │     R0     │     R8     │
 * │     R1     │     R9     │
 * │     R2     │     R10    │
 * │     R3     │     R11    │
 * │     R4     │     R12    │
 * │     R5     │     R13    │
 * │     R6     │     R14    │
 * │     R7     │     R15    │
 * └────────────┴────────────┘
 */

/**
 * @brief Colour depth in bits
 * 
 * Must be between 1 and 8. 
 * Note that turning this up increases the memory usage for the framebuffer
 * and at some point results in flickering. 
 */
#define COLOUR_DEPTH 6

/**
 * @brief Initialises the driver
 * 
 * Must be called before any other led*() function.
 */
void ledInit(void);

/**
 * @brief Starts the driver
 * @details This starts the timer and actually lights up the LEDs set by
 * ledSet(). Interrupts must be enabled globally separately. 
 */
void ledOn(void);

/**
 * @brief Stops the driver
 * @details This stops the timer and turns off all LEDs (anything set by
 * ledSet() is remembered for when ledOn() is called) to save as much power as
 * possible. 
 */
void ledOff(void);

/**
 * @brief Sets the value for one LED
 * @param x,y Coordinates of the LED (0..7).
 * @param value The brightness value of the LED (0..255 but only the highest
 * COLOUR_DEPTH many bits are relevant)
 */
void ledSet(uint8_t x, uint8_t y, uint8_t value);

/**
 * @brief Sets a value for all LEDs
 * @param value The brightness value of the LED (0..255 but only the highest
 * COLOUR_DEPTH many bits are relevant)
 */
void ledSetAll(uint8_t value);

#endif // LED_H

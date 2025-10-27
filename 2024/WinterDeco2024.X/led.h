/**
 * @file led.h
 * @date 2024-10-06
 * @brief Driver for multiplexed LEDs
 * 
 * This driver controls the 24 LEDs in a 4x3 matrix configuration (4 rows,
 * 3 columns) with two LEDs (one forward, one backward) in each cell of the
 * matrix. The rows are on RC[0:2] and the columns on RC[3:6]. 
 * Timer 0 is used for timing the multiplexing. 
 */

#ifndef LED_H
#define	LED_H

#include<stdint.h>

/* 
 * Positions of the LEDs:
 *      ----------
 *      |11    23|
 *      |        |
 *      |        |
 *    --------------
 *     /  0    3  \
 *    /     12     \
 *    |    4  19   |
 *    |  8      15 |
 *    \    16  7   /
 *    /            \
 *   / 5    20   17 \
 *  /|      9       |\
 * / |      21      | \
 * \ |      14      | /
 *  \|      2       |/
 *   \ 1         13 /
 *    \            /
 *    /            \
 *   /              \
 *  / 18          10 \
 *  |                |
 *  |                |
 *  |                |
 *  |                |
 *  \                /
 *   \ 6         22 /
 *    \____________/
 */
#define LED_HAT_LEFT 11
#define LED_HAT_RIGHT 23
#define LED_EYE_LEFT 0
#define LED_EYE_RIGHT 3
#define LED_NOSE 12
#define LED_UPPER_LIP_LEFT 4
#define LED_UPPER_LIP_RIGHT 19
#define LED_MOUTH_LEFT 8
#define LED_MOUTH_RIGHT 15
#define LED_LOWER_LIP_LEFT 16
#define LED_LOWER_LIP_RIGHT 7
#define LED_SHOULDER_LEFT 5
#define LED_SHOULDER_RIGHT 17
#define LED_BUTTON_1 20
#define LED_BUTTON_2 9
#define LED_BUTTON_3 21
#define LED_BUTTON_4 14
#define LED_BUTTON_5 2
#define LED_HAND_LEFT 1
#define LED_HAND_RIGHT 13
#define LED_KNEE_LEFT 18
#define LED_KNEE_RIGHT 10
#define LED_FOOT_LEFT 6
#define LED_FOOT_RIGHT 22

/**
 * @brief Colour depth in bits
 * 
 * Must be between 1 and 8. 
 * Note that turning this up increases the memory usage for the "frame"buffer
 * and at some point results in flickering. 
 */
#define COLOUR_DEPTH 6

/**
 * @brief Determines what the eighth TRIS and LAT bits should be set to. 
 * 
 * In order to optimise the ISR, whole bytes are written to TRISC and LATC. 
 * Thus the unused eighth pin RC7 is also set each time. Masking this out would
 * result in an unacceptable decrease in speed. 
 * 
 * In this case, RC7 is one of the guard ring outputs, i.e. the alternate
 * function overrides the LAT bit (but not the TRIS bit). 
 */
#define TRIS_C7 0 // Set to output for guard ring
#define LAT_C7 0 // This one doesn't matter

/**
 * @brief Initialises the driver
 * 
 * Must be called before any other led*() function.
 */
void ledInit(void);

/**
 * @brief Starts the driver
 * @details This starts the timer and actually lights up the LEDs set by
 * ledSet(). 
 */
void ledOn(void);

/**
 * @brief Stops the driver
 * @details This stops the timer and turns off all LEDs (anything set by
 * ledSet() is remembered for when ledOn() is called) to save as much power as
 * possible. Interrupts must be enabled globally separately. 
 */
void ledOff(void);

/**
 * @brief Sets the value for one LED
 * @param led The number of the LED (0..23)
 * @param value The brightness value of the LED (0..255 but only the highest
 * COLOUR_DEPTH many bits are relevant)
 */
void ledSet(uint8_t led, uint8_t value);

/**
 * @brief Sets a value for all LEDs
 * @param value The brightness value of the LED (0..255 but only the highest
 * COLOUR_DEPTH many bits are relevant)
 */
void ledSetAll(uint8_t value);

#endif // LED_H

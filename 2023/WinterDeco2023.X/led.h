/**
 * @file led.h
 * @date 2024-10-06
 * @brief Driver for multiplexed LEDs
 * 
 * This driver controls the 30 LEDs in a 5x3 matrix configuration (5 rows,
 * 3 columns, 2 LEDs (forward and backward-facing) in each matrix entry).
 * The rows are on RC[3:7] and the columns on RC[0:2]. 
 * Timer 0 is used for timing the multiplexing. 
 */

#ifndef LED_H
#define	LED_H

#include<stdint.h>

/* 
 * Positions of the LEDs
 * Front (the side with the battery, microcontroller, and touch sensor):
 *                    ╱╲
 *                  ╱  0 ╲
 *                ╱ 15     ╲
 *              ╱        1   ╲
 *            ╱  2     17      ╲
 *           ⎯⎯              ⎯⎯
 *            ╱   16       4   ╲
 *          ╱                    ╲
 *        ╱   6                    ╲
 *      ╱             3         5    ╲
 *     ⎯⎯⎯                       ⎯⎯⎯
 *        ╱   21            [T]    ╲
 *      ╱                            ╲
 *    ╱           22                   ╲
 *  ╱   7                   8       23   ╲
 * ⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯        ⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯
 *                 ⏐       ⏐
 *                 ⏐       ⏐
 *                 ⎯⎯⎯⎯⎯
 * 
 * Back (the side with only LEDs):
 *                     ╱╲
 *                   ╱    ╲
 *                 ╱  9     ╲
 *               ╱       24   ╲
 *             ╱  11         10 ╲
 *            ⎯⎯   26         ⎯⎯
 *             ╱         25     ╲
 *           ╱                    ╲
 *         ╱       20        12     ╲
 *       ╱ 18                         ╲
 *      ⎯⎯⎯   19        28        ⎯⎯⎯
 *         ╱                        ╲
 *       ╱                            ╲
 *     ╱                   13           ╲
 *   ╱    29       14              27     ╲
 *  ⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯        ⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯
 *                  ⏐       ⏐
 *                  ⏐       ⏐
 *                  ⎯⎯⎯⎯⎯
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
 * @param led Number of the LED (0..29).
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

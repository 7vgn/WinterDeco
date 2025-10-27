/**
 * @file programs.c
 * @date 2023-12-20
 * @brief Implements programs.h
 */

#include<xc.h>
#include"led.h"
#include"programs.h"

/**
 * @brief VERY simple (and terrible) PRNG
 */
uint8_t random(uint8_t max)
{
	static uint8_t seed;
	seed = (seed * 35 + 87);
	return seed % max;
}

/**
 * @brief Dummy function that does nothing
 */
void null(uint16_t clk) {}

/**
 * @brief Program function for "All on"
 */
void programAllOn(uint16_t clk)
{
	for(uint8_t led = 0; led < 30; led++)
		ledSet(led, 0xff);
}

/**
 * @brief Program function for "Fast blink"
 */
void programFastBlink(uint16_t clk)
{
	// 80ms is fast enough
	if(clk & 0b111) return;
	// Have at most 6 LEDs on at any time
	ledSetAll(0);
	for(uint8_t i = 0; i < 6; i++)
		ledSet(random(30), 0xff);
}

/**
 * @brief Program function for "Snowfall"
 */
void programSnowfall(uint16_t clk)
{
	// Slow down to 160ms
	if((clk & 0b1111) == 0)
	{
		switch((clk >> 4) & 0b111)
		{
		case 0:
			ledSetAll(0);
			ledSet(0, 255); ledSet(3, 255); ledSet(6, 255);
			ledSet(9, 255); ledSet(12, 255); ledSet(20, 255);
			break;
		case 1:
			ledSetAll(0);
			ledSet(5, 255); ledSet(1, 255); ledSet(15, 255);
			ledSet(21, 255); ledSet(19, 255); ledSet(24, 255);
			ledSet(28, 255); ledSet(18, 255); ledSet(11, 255);
			break;
		case 2:
			ledSetAll(0);
			ledSet(8, 255); ledSet(23, 255); ledSet(17, 255);
			ledSet(2, 255); ledSet(7, 255); ledSet(22, 255);
			ledSet(14, 255); ledSet(26, 255); ledSet(27, 255);
			ledSet(10, 255); ledSet(13, 255); ledSet(29, 255);
			break;
		case 3:
			ledSetAll(0);
			ledSet(4, 255); ledSet(16, 255); ledSet(25, 255);
			break;
		case 4:
			ledSetAll(0);
			ledSet(3, 255); ledSet(0, 255); ledSet(6, 255);
			ledSet(20, 255); ledSet(9, 255); ledSet(12, 255);
			break;
		case 5:
			ledSetAll(0);
			ledSet(5, 255); ledSet(1, 255); ledSet(15, 255);
			ledSet(21, 255); ledSet(19, 255); ledSet(24, 255);
			ledSet(28, 255); ledSet(18, 255); ledSet(11, 255);
			break;
		case 6:
			ledSetAll(0);
			ledSet(8, 255); ledSet(23, 255); ledSet(17, 255);
			ledSet(2, 255); ledSet(7, 255); ledSet(22, 255);
			ledSet(14, 255); ledSet(26, 255); ledSet(27, 255);
			ledSet(10, 255); ledSet(13, 255); ledSet(29, 255);
			break;
		case 7:
			ledSetAll(0);
			ledSet(4, 255); ledSet(16, 255); ledSet(12, 255);
			break;
		}
	}
}

/**
 * @brief Program function for "Flickering"
 */
void programFlickering(uint16_t clk)
{
	if(clk & 0b11)
		return;
	if(clk & 0xc)
		ledSetAll(0xff);
	else
	{
		// Have at most 3 LEDs off at any time
		ledSet(random(30), 0);
		ledSet(random(30), 0);
		ledSet(random(30), 0);
	}
}

/**
 * @brief Program function for "Snake"
 */
void programSnake(uint16_t clk)
{
	// Slow down to 40ms
	if(!(clk & 0b11))
	{
		ledSetAll(0);
		switch((clk >> 2) % 15)
		{
		case  0:
			ledSet(8, 255); ledSet(23, 255); ledSet(3, 255);
			ledSet(5, 255); ledSet(20, 255); ledSet(26, 255);
			ledSet(9, 255); ledSet(11, 255);
			break;
		case  1:
			ledSet(23, 255); ledSet(3, 255); ledSet(5, 255);
			ledSet(4, 255); ledSet(20, 255); ledSet(26, 255);
			ledSet(18, 255); ledSet(11, 255);
			break;
		case  2:
			ledSet(3, 255); ledSet(5, 255); ledSet(17, 255);
			ledSet(4, 255); ledSet(19, 255); ledSet(20, 255);
			ledSet(26, 255); ledSet(18, 255);
			break;
		case  3:
			ledSet(3, 255); ledSet(1, 255); ledSet(17, 255);
			ledSet(4, 255); ledSet(19, 255); ledSet(20, 255);
			ledSet(18, 255); ledSet(29, 255);
			break;
		case  4:
			ledSet(1, 255); ledSet(17, 255); ledSet(0, 255);
			ledSet(4, 255); ledSet(19, 255); ledSet(14, 255);
			ledSet(18, 255); ledSet(29, 255);
			break;
		case  5:
			ledSet(1, 255); ledSet(17, 255); ledSet(0, 255);
			ledSet(15, 255); ledSet(19, 255); ledSet(14, 255);
			ledSet(28, 255); ledSet(29, 255);
			break;
		case  6:
			ledSet(1, 255); ledSet(0, 255); ledSet(15, 255);
			ledSet(2, 255); ledSet(14, 255); ledSet(28, 255);
			ledSet(13, 255); ledSet(29, 255);
			break;
		case  7:
			ledSet(0, 255); ledSet(15, 255); ledSet(2, 255);
			ledSet(16, 255); ledSet(14, 255); ledSet(28, 255);
			ledSet(27, 255); ledSet(13, 255);
			break;
		case  8:
			ledSet(15, 255); ledSet(2, 255); ledSet(6, 255);
			ledSet(16, 255); ledSet(28, 255); ledSet(27, 255);
			ledSet(13, 255); ledSet(12, 255);
			break;
		case  9:
			ledSet(2, 255); ledSet(6, 255); ledSet(16, 255);
			ledSet(21, 255); ledSet(27, 255); ledSet(25, 255);
			ledSet(13, 255); ledSet(12, 255);
			break;
		case 10:
			ledSet(6, 255); ledSet(16, 255); ledSet(21, 255);
			ledSet(7, 255); ledSet(27, 255); ledSet(10, 255);
			ledSet(25, 255); ledSet(12, 255);
			break;
		case 11:
			ledSet(6, 255); ledSet(21, 255); ledSet(7, 255);
			ledSet(22, 255); ledSet(24, 255); ledSet(10, 255);
			ledSet(25, 255); ledSet(12, 255);
			break;
		case 12:
			ledSet(8, 255); ledSet(21, 255); ledSet(7, 255);
			ledSet(22, 255); ledSet(24, 255); ledSet(10, 255);
			ledSet(25, 255); ledSet(9, 255);
			break;
		case 13:
			ledSet(8, 255); ledSet(23, 255); ledSet(7, 255);
			ledSet(22, 255); ledSet(24, 255); ledSet(10, 255);
			ledSet(9, 255); ledSet(11, 255);
			break;
		case 14:
			ledSet(8, 255); ledSet(23, 255); ledSet(5, 255);
			ledSet(22, 255); ledSet(24, 255); ledSet(26, 255);
			ledSet(9, 255); ledSet(11, 255);
			break;
		}
	}
}

/**
 * @brief Program function for "Slow blink"
 */
void programSlowBlink(uint16_t clk)
{
	for(uint8_t led = 0; led < 30; led++)
		ledSet(led, 255 - (uint8_t)(3 * clk + 8 * led));
}

/**
 * @brief Array containing all implemented programs
 */
const Program PROGRAMS[] = {
	{"Slow blink", null, programSlowBlink},
	{"All on", programAllOn, null},
	{"Fast blink", null, programFastBlink},
	{"Snowfall", null, programSnowfall},
	{"Flickering", null, programFlickering},
	{"Snake", null, programSnake}
};
const uint8_t NUMBER_OF_PROGRAMS = (sizeof(PROGRAMS) / sizeof(Program));
/**
 * @file led.c
 * @date 2024-10-06
 * @brief Implements led.h
 * 
 * Assignment of LED numbers to rows/columns:
 * LEDs 0..14 are "forward", i.e. anode at column, cathode at row. 
 * 
 *  LED │ Row │ Col 
 * ─────┼─────┼─────
 *    0 │   1 │   1 
 *    1 │   1 │   2 
 *    2 │   1 │   3 
 *    3 │   2 │   1 
 *    4 │   2 │   2 
 *    5 │   2 │   3 
 *    6 │   3 │   1 
 *    7 │   3 │   2 
 *    8 │   3 │   3 
 *    9 │   4 │   1 
 *   10 │   4 │   2 
 *   11 │   4 │   3 
 *   12 │   5 │   1 
 *   13 │   5 │   2 
 *   14 │   5 │   3 
 * 
 * LEDs 15..29 are "backward", i.e. anode at row, cathode at column.
 *  LED │ Row │ Col 
 * ─────┼─────┼─────
 *   15 │   1 │   1 
 *   16 │   1 │   2 
 *   17 │   1 │   3 
 *   18 │   2 │   1 
 *   19 │   2 │   2 
 *   20 │   2 │   3 
 *   21 │   3 │   1 
 *   22 │   3 │   2 
 *   23 │   3 │   3 
 *   24 │   4 │   1 
 *   25 │   4 │   2 
 *   26 │   4 │   3 
 *   27 │   5 │   1 
 *   28 │   5 │   2 
 *   29 │   5 │   3 
 */

#include<xc.h>
#include"led.h"

/**
 * @brief The framebuffer for the LEDs
 * 
 * The buffer has a separate plane for each bit of the colour depth. 
 * Each plane stores the TRISC and LATC values for each row. 
 * Index 0..4 is for the forward-facing LEDs in Rows 1..5.
 * Index 0..9 is for the same rows but the backward-facing LEDs. 
 */
static volatile struct
{
	uint8_t tris;
	uint8_t lat;
} buffer[COLOUR_DEPTH][10];

/**
 * @brief Multiplexing sequence for LEDs
 * 
 * During each iteration, Plane i+1 is shown twice as often as Plane i. 
 * This sequence determines which plane is shown when. 
 */
#define SEQUENCE_LENGTH ((1 << COLOUR_DEPTH) - 1)
static uint8_t planeSequence[SEQUENCE_LENGTH];

/**
 * @brief The current position in the sequence
 */
static volatile uint8_t currentSeqPos;

/**
 * @brief The current row
 */
static volatile uint8_t currentRow;

void ledInit()
{
	// Initialise plane sequence
	// The plane sequence contains plane p (0..COLOUR_DEPTH-1) 2^p times for a
	// total sequence length of 2^COLOR_DEPTH-1. It needs to be sufficiently
	// "mixed" to avoid flickering, e.g. for COLOUR_DEPTH=3 we want
	// (2,1,2,0,2,1,2) rather than (0,1,1,2,2,2,2). 
	// To generate this, we go through the planes in descending order and insert
	// each plane p at every k-th place (where k = 2^(COLOUR_DEPTH - 1 - p)),
	// starting at offset k - 1. 
	for(int p = COLOUR_DEPTH - 1; p >= 0; p--)
	{
		int k = 1 << (COLOUR_DEPTH - 1 - p);
		for(int i = k - 1; i < SEQUENCE_LENGTH; i += k)
			planeSequence[i] = (uint8_t)p;
	}
	// Initialise buffer
	for(uint8_t plane = 0; plane < COLOUR_DEPTH; plane++)
	{
		for(uint8_t row = 0; row < 5; row++)
		{
			// Forward rows (LED on when col=1 and row=0)
			buffer[plane][row].tris = (uint8_t)((~(1 << row)) << 3);
			buffer[plane][row].lat = 0;
			// Backward rows (LED on when row=1 and col=0)
			buffer[plane][row + 5].tris = (uint8_t)((~(1 << row)) << 3);
			buffer[plane][row + 5].lat = 0xff;
		}
	}
	
	currentSeqPos = 0;
	currentRow = 0;
	
	// Turn everything off initially
	ledOff();
}

void ledOn(void)
{
	// Set up timer and enable interrupt
	T0CON0bits.MD16 = 0;		// Operate in 8-bit mode
	T0CON0bits.OUTPS = 0b0000;	// Postscaler 1:1
	T0CON1bits.CS = 0b010;		// Clock Source F_OSC/4 = 16Mhz
	T0CON1bits.CKPS = 0b0000;	// Prescaler 1:1
	TMR0H = 250;				// Compare value (-> 64kHz)
	PIE3bits.TMR0IE = 1;		// Enable interrupt on compare match
	T0CON0bits.EN = 1;
}

void ledOff(void)
{
	// Stop timer and disable interrupt
	PIE3bits.TMR0IE = 0;
	T0CON0bits.EN = 0;
	
	// Turn all LEDs off by settings all pins to output, low
	LATC = 0;
	TRISC = 0;
}

void ledSet(uint8_t led, uint8_t value)
{
	di();
	uint8_t row, col;
	value >>= 8 - COLOUR_DEPTH; // Ignore all but the leftmost COLOUR_DEPTH many bits
	if(led < 15)
	{
		// This is a forward LED
		row = led / 3;
		col = led % 3;
	}
	else
	{
		// This is a backward LED
		row = 5 + (led - 15) / 3;
		col = led % 3;
		value = ~value; // For backward LEDs, the column bits are inverted, see ledInit()
	}
	for(uint8_t plane = 0; plane < COLOUR_DEPTH; plane++)
	{
		// Extract plane-th last bit from value
		uint8_t bit = (value >> plane) & 1u;
		// Write this into the col-th last bit of lat
		buffer[plane][row].lat = (buffer[plane][row].lat & ~(1 << col)) | (uint8_t)(bit << col);
	}
	ei();
}

void ledSetAll(uint8_t value)
{
	for(uint8_t led = 0; led < 30; led++)
		ledSet(led, value);
}

/**
 * @brief Interrupt handler for Timer 0
 */
void __interrupt(irq(IRQ_TMR0), high_priority) timer0Isr(void)
{
	// Increment currentRow and - if necessary - currentSeqPos
	currentRow++;
	if(currentRow == 10)
	{
		currentRow = 0;
		currentSeqPos++;
		if(currentSeqPos == SEQUENCE_LENGTH)
			currentSeqPos = 0;
	}

	// Tri-state all rows while new column data is applied
	TRISC = 0xff;
	
	// Select the row and apply column values
	LATC = buffer[planeSequence[currentSeqPos]][currentRow].lat;
	
	// Configure current row and all columns as outputs
	TRISC = buffer[planeSequence[currentSeqPos]][currentRow].tris;

	// Clear interrupt
	TMR0IF = 0;
}

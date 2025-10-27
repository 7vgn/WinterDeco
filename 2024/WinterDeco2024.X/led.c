/**
 * @file led.c
 * @date 2024-10-06
 * @brief Implements led.h
 */

#include<xc.h>
#include"led.h"

/**
 * @brief The framebuffer for the LEDs
 * 
 * The buffer has a separate plane for each bit of the colour depth. 
 * Each plane stores TRISC and LATC values for each row. 
 * Rows 0..2 are the forward-facing LEDs, i.e. anode at column, cathode at row.
 * Rows 3..5 are same as Rows 0..2 but for the backward-facing LEDs, i.e. anode
 * at row, cathode at column.
 */
static volatile struct
{
	uint8_t tris;
	uint8_t lat;
} buffer[COLOUR_DEPTH][6];

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
		for(uint8_t row = 0; row < 3; row++)
		{
			// Forward rows (LED on when row=0, col=1)
			buffer[plane][row].tris = (TRIS_C7 << 7) | (uint8_t)(~(1 << row) & 0b111);
			buffer[plane][row].lat  = (LAT_C7 << 7);
			// Backward rows (LED on when row=1, col=0)
			buffer[plane][row + 3].tris = (TRIS_C7 << 7) | (uint8_t)(~(1 << row) & 0b111);
			buffer[plane][row + 3].lat  = (uint8_t)(LAT_C7 << 7) | 0x3f;
			// LEDs can now be turned on by setting LATC[3:6] high (for
			// forward rows) or low (for backward rows). 
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
	LATC = (LAT_C7 << 7);
	TRISC = (TRIS_C7 << 7);
}

void ledSet(uint8_t led, uint8_t value)
{
	di();
	uint8_t row, col;
	value >>= 8 - COLOUR_DEPTH; // Ignore all but the leftmost COLOUR_DEPTH many bits
	if(led < 12)
	{
		// This is a forward LED
		row = led / 4;
		col = led % 4;
	}
	else
	{
		// This is a backward LED
		row = 3 + (led - 12) / 4;
		col = led % 4;
		value = ~value; // For the backward columns, the row bits are inverted, see ledInit()
	}
	for(uint8_t plane = 0; plane < COLOUR_DEPTH; plane++)
	{
		// Extract plane-th last bit from value
		uint8_t bit = (value >> plane) & 1u;
		// Write this into the (col+3)-th last bit of lat
		buffer[plane][row].lat = (buffer[plane][row].lat & ~(1 << (3 + col))) | (uint8_t)(bit << (3 + col));
	}
	ei();
}

void ledSetAll(uint8_t value)
{
	for(uint8_t led = 0; led < 24; led++)
		ledSet(led, value);
}

/**
 * @brief Interrupt handler for Timer 0
 */
void __interrupt(irq(IRQ_TMR0), high_priority) timer0Isr(void)
{
	// Increment currentRow and - if necessary - currentSeqPos
	currentRow++;
	if(currentRow == 6)
	{
		currentRow = 0;
		currentSeqPos++;
		if(currentSeqPos == SEQUENCE_LENGTH)
			currentSeqPos = 0;
	}

	// Make all rows High-z while new column data is applied
	TRISC = (TRIS_C7 << 7) | 0b01111111;
	
	// Select the row and apply column values
	LATC = buffer[planeSequence[currentSeqPos]][currentRow].lat;
	
	// Configure current row and all columns as outputs
	TRISC = buffer[planeSequence[currentSeqPos]][currentRow].tris;

	// Clear interrupt
	TMR0IF = 0;
}

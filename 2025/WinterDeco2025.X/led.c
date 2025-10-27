/**
 * @file led.c
 * @date 2025-10-21
 * @brief Implements led.h
 */

#include<xc.h>
#include"led.h"

/**
 * @brief The framebuffer for the LEDs
 * 
 * The buffer has a separate plane for each bit of the colour depth. 
 * Each plane stores the LATC values for each row. 
 */
static volatile struct
{
	uint8_t lat;
} buffer[COLOUR_DEPTH][16];

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
		for(uint8_t row = 0; row < 16; row++)
			buffer[plane][row].lat = (uint8_t)(row << 4);
	
	currentSeqPos = 0;
	currentRow = 0;
	
	// Configure RC[0:7] and RB7 as outputs
	TRISC = 0;
	TRISBbits.TRISB7 = 0;
	
	// Turn everything off initially
	ledOff();
}

void ledOn(void)
{
	// Set up timer and enable interrupt
	T0CON0bits.MD16 = 0; // Operate in 8-bit mode
	T0CON0bits.OUTPS = 0b0000; // Postscaler 1:1
	T0CON1bits.CS = 0b010; // Clock Source F_OSC/4 = 16Mhz
	T0CON1bits.CKPS = 0b0000; // Prescaler 1:1
	TMR0H = 250; // Compare value (-> 64kHz)
	PIE3bits.TMR0IE = 1; // Enable interrupt on compare match
	T0CON0bits.EN = 1;
}

void ledOff(void)
{
	// Disable demux
	LATBbits.LATB7 = 1;
	
	// Stop timer and disable interrupt
	PIE3bits.TMR0IE = 0;
	T0CON0bits.EN = 0;
	
	// Turn all LEDs off by settings all pins low
	LATC = 0;
}

void ledSet(uint8_t x, uint8_t y, uint8_t value)
{
	di();
	if(x & 4u)
	{
		x &= ~4u;
		y |= 8u;
	}
	value >>= 8 - COLOUR_DEPTH; // Ignore all but the leftmost COLOUR_DEPTH many bits
	for(uint8_t plane = 0; plane < COLOUR_DEPTH; plane++)
	{
		// Extract plane-th last bit from value
		uint8_t bit = (value >> plane) & 1u;
		// Write this into the x-th last bit of lat
		buffer[plane][y].lat = (buffer[plane][y].lat & ~(1 << x)) | (uint8_t)(bit << x);
	}
	ei();
}

void ledSetAll(uint8_t value)
{
	for(uint8_t y = 0; y < 8; y++)
		for(uint8_t x = 0; x < 8; x++)
			ledSet(x, y, value);
}

/**
 * @brief Interrupt handler for Timer 0
 */
void __interrupt(irq(IRQ_TMR0), high_priority) timer0Isr(void)
{
	// Increment currentRow and - if necessary - currentSeqPos
	currentRow++;
	if(currentRow == 16)
	{
		currentRow = 0;
		currentSeqPos++;
		if(currentSeqPos == SEQUENCE_LENGTH)
			currentSeqPos = 0;
	}

	// Disable row demux while new column data is applied
	LATBbits.LATB7 = 1;
	
	// Select the row and apply column values
	LATC = buffer[planeSequence[currentSeqPos]][currentRow].lat;
	
	// Re-enable row demux
	LATBbits.LATB7 = 0;

	// Clear interrupt
	TMR0IF = 0;
}

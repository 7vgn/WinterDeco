/**
 * @file uart.c
 * @date 2024-10-06
 * @brief Implementation of uart.h
 */

#include<xc.h>
#include<stdio.h>
#include"uart.h"

void uartInit(void)
{
	// Set up baud rate generator:
	U1CON0bits.BRGS = 0;		// Normal baud rate: F_OSC/(16*(U2BRG+1))
	U1BRG = 15;					// = 64MHz/(16*(15+1)) = 250kBaud

	// Enable USART module
	U1CON0bits.MODE = 0b0000;	// Asynchronous 8-bit w/o parity
	U1CON1bits.ON = 1;			// Enable USART
	U1CON0bits.TXEN = 1;		// Enable transmitter
	
	// Configure Pin RA2 to output UART 1 TX
	RA2PPS = 0x10;				// UART1_TX
	TRISAbits.TRISA2 = 0;		// Direction: Output
}

void uartSend(char c)
{
	// Wait for transmit buffer to become empty
	while(PIR4bits.U1TXIF == 0);
	// Copy c to transmit buffer
	U1TXB = c;
}

void uartFlush(void)
{
	// Wait until everything from the transmit buffer and the transmit shift
	// register has been sent
	while(U1ERRIRbits.TXMTIF == 0);
}

/**
 * @brief Redirect printf() output to UART
 * 
 * This function is called by stdio to output a character. Implementing it
 * enables printf() functionality. 
 * @param c Character to send
 */
void putch(char c)
{
	uartSend(c);
}

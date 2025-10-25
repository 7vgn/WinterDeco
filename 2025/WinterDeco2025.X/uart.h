/**
 * @file uart.h
 * @date 2024-10-06
 * @brief Primitive serial driver (output only) for PIC18F14Q41
 * 
 * Only implements 8-bit data mode @250kBaud
 * Has printf() functionality
 */

#ifndef UART_H
#define	UART_H

/**
 * @brief Initialises the driver
 */
void uartInit(void);

/**
 * @brief Transmit a byte over UART
 * 
 * While this function is non-blocking, it will still busy wait until the
 * transmit buffer is free. After writing the byte to the transmit buffer, it
 * will however not wait for it to be transmitted. 
 * @param c The byte to be transmitted
 */
void uartSend(char c);

/**
 * @brief Wait until everything has been transmitted
 */
void uartFlush(void);

#endif	/* UART_H */


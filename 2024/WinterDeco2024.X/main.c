/**
 * @file main.c
 * @date 2024-10-06
 * @brief WinterDeco main program
 */

/**
 * @brief PIC18F14Q41 Configuration Bit Settings
 */
// CONFIG1
#pragma config FEXTOSC = OFF    // External Oscillator Selection (Oscillator not enabled)
#pragma config RSTOSC = HFINTOSC_64MHZ// Reset Oscillator Selection (HFINTOSC with HFFRQ = 64 MHz and CDIV = 1:1)
// CONFIG2
#pragma config CLKOUTEN = OFF   // Clock out Enable bit (CLKOUT function is disabled)
#pragma config PR1WAY = ON      // PRLOCKED One-Way Set Enable bit (PRLOCKED bit can be cleared and set only once)
#pragma config CSWEN = ON       // Clock Switch Enable bit (Writing to NOSC and NDIV is allowed)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor enabled)
#pragma config FCMENP = ON      // Fail-Safe Clock Monitor - Primary XTAL Enable bit (Fail-Safe Clock Monitor enabled; timer will flag FSCMP bit and OSFIF interrupt on EXTOSC failure.)
#pragma config FCMENS = ON      // Fail-Safe Clock Monitor - Secondary XTAL Enable bit (Fail-Safe Clock Monitor enabled; timer will flag FSCMP bit and OSFIF interrupt on SOSC failure.)
// CONFIG3
#pragma config MCLRE = EXTMCLR  // MCLR Enable bit (If LVP = 0, MCLR pin is MCLR; If LVP = 1, RE3 pin function is MCLR )
#pragma config PWRTS = PWRT_OFF // Power-up timer selection bits (PWRT is disabled)
#pragma config MVECEN = ON      // Multi-vector enable bit (Multi-vector enabled, Vector table used for interrupts)
#pragma config IVT1WAY = ON     // IVTLOCK bit One-way set enable bit (IVTLOCKED bit can be cleared and set only once)
#pragma config LPBOREN = OFF    // Low Power BOR Enable bit (Low-Power BOR disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled , SBOREN bit is ignored)
// CONFIG4
#pragma config BORV = VBOR_1P9  // Brown-out Reset Voltage Selection bits (Brown-out Reset Voltage (VBOR) set to 1.9V)
#pragma config ZCD = OFF        // ZCD Disable bit (ZCD module is disabled. ZCD can be enabled by setting the ZCDSEN bit of ZCDCON)
#pragma config PPS1WAY = ON     // PPSLOCK bit One-Way Set Enable bit (PPSLOCKED bit can be cleared and set only once; PPS registers remain locked after one clear/set cycle)
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = ON         // Low Voltage Programming Enable bit (Low voltage programming enabled. MCLR/VPP pin function is MCLR. MCLRE configuration bit is ignored)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Extended Instruction Set and Indexed Addressing Mode disabled)
// CONFIG5
#pragma config WDTCPS = WDTCPS_31// WDT Period selection bits (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = SWDTEN    // WDT operating mode (WDT enabled/disabled by SWDTEN bit)
// CONFIG6
#pragma config WDTCWS = WDTCWS_7// WDT Window Select bits (window always open (100%); software control; keyed access not required)
#pragma config WDTCCS = SC      // WDT input clock selector (Software Control)
// CONFIG7
#pragma config BBSIZE = BBSIZE_512// Boot Block Size selection bits (Boot Block size is 512 words)
#pragma config BBEN = OFF       // Boot Block enable bit (Boot block disabled)
#pragma config SAFEN = OFF      // Storage Area Flash enable bit (SAF disabled)
#pragma config DEBUG = OFF      // Background Debugger (Background Debugger disabled)
// CONFIG8
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block not Write protected)
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers not Write protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not Write protected)
#pragma config WRTSAF = OFF     // SAF Write protection bit (SAF not Write Protected)
#pragma config WRTAPP = OFF     // Application Block write protection bit (Application Block not write protected)
// CONFIG9
#pragma config CP = OFF         // PFM and Data EEPROM Code Protection bit (PFM and Data EEPROM code protection disabled)

#include<xc.h>
#include<stdbool.h>
#include<stdio.h>
#include"uart.h"
#include"led.h"
#include"battery.h"
#include"touch.h"
#include"input.h"
#include"programs.h"

/**
 * @brief Tick flag for system clock
 * @details Set by the Timer 2 interrupt every 10ms, cleared by main.
 */
volatile bool tick = false;

/**
 * @brief Timer 2 interrupt service routine
 */
void __interrupt(irq(TMR2), low_priority) timer2Isr(void)
{
	// Set tick flag
	tick = true;
	// Reset interrupt flag
	PIR3bits.TMR2IF = 0;
}

/**
 * @brief Sleeps until the right foot sensor is touched for at least 2s
 * 
 * Uses the watchdog timer to sleep for 1s at a time. After each sleep interval
 * the status of the touch sensor is checked. If it is active on two
 * consecutive checks, we stop sleeping and wait until the sensor is released
 * before returning. 
 */
void sleepUntilTouch(void)
{
	// Turn off system clock
	T2CONbits.ON = 0;

	// Signal going-to-sleep status by lighting only the eyes until the sensor
	// is released
	ledSetAll(0x00);
	ledSet(LED_EYE_LEFT, 0xff);
	ledSet(LED_EYE_RIGHT, 0xff);
	printf("Going to sleep...");
	while(isTouched(SENSOR_FOOT_RIGHT))
		__delay_ms(50);
	printf("Zzz\n");
	uartFlush();
	
	// Turn off LED driver
	ledOff();

	// Go to sleep until touch event occurs
	uint8_t touch = 0;
	while(1)
	{
		// Select sleep mode
		CPUDOZEbits.IDLEN = 0; // Sleep instead of just idle
		// Start Watchdog Timer
		WDTCON0bits.SEN = 1;
		// Zzzz...
		// Entering sleep clears the WDT. 
		// During sleep, the Watchdog causes only wake-up, no reset. 
		SLEEP();
		// Disable Watchdog Timer to prevent reset
		// (First instruction after sleep is guaranteed to be executed
		// immediately even if an interrupt is pending)
		WDTCON0bits.SEN = 0; // This also resets the counter for the next time
		
		// Check if sensor is touched
		touch = (uint8_t)(touch << 1) | (isTouched(SENSOR_FOOT_RIGHT) ? 1 : 0);
		if((touch & 0b11) == 0b11)
			// Sensor has been touched for at least 2s in a row
			break;
	}

	// Waking up: Signal wake-up status by lighting the eyes until the sensor is
	// released
	ledOn();
	printf("Waking up...");
	while(isTouched(SENSOR_FOOT_RIGHT))
		__delay_ms(50);
	ledSetAll(0x00);
	printf("I'm up!\n");

	// Turn on system clock
	T2CONbits.ON = 1;
}

/**
 * @brief Main function
 */
void main(void)
{
	// Disable unused peripheral modules
	// Used peripherals: Timer 0, Timer 1, UART 1, ADC, Fixed Voltage Reference
	PMD0bits.CRCMD = 1;
	PMD0bits.SCANMD = 1;
	PMD1bits.CM1MD = 1;
	PMD1bits.ZCDMD = 1;
	PMD1bits.SMT1MD = 1;
	PMD1bits.TMR1MD = 1;
	PMD1bits.TMR3MD = 1;
	PMD1bits.TMR4MD = 1;
	PMD2bits.CCP1MD = 1;
	PMD2bits.CWG1MD = 1;
	PMD2bits.DSM1MD = 1;
	PMD2bits.NCO1MD = 1;
	PMD2bits.ACTMD = 1;
	PMD2bits.DAC1MD = 1;
	PMD3bits.U2MD = 1;
	PMD3bits.SPI1MD = 1;
	PMD3bits.SPI2MD = 1;
	PMD3bits.I2C1MD = 1;
	PMD3bits.PWM1MD = 1;
	PMD3bits.PWM2MD = 1;
	PMD3bits.PWM3MD = 1;
	PMD4bits.DMA1MD = 1;
	PMD4bits.DMA2MD = 1;
	PMD4bits.DMA3MD = 1;
	PMD4bits.CLC1MD = 1;
	PMD4bits.CLC2MD = 1;
	PMD4bits.CLC3MD = 1;
	PMD4bits.CLC4MD = 1;
	PMD4bits.U3MD = 1;
	PMD5bits.DAC2MD = 1;
	PMD5bits.DMA4MD = 1;
	PMD5bits.OPAMD = 1;
	
	// Initialise WDT
	WDTCON0bits.SEN = 0;		// Disable WDT for now
	WDTCON0bits.PS = 0b01010;	// WDT Prescaler: 1:32768 (1s interval)
	WDTCON1bits.CS = 0b000;		// WDT clock source. LFINTOSC (31kHz)
	
	// Initialise UART
	uartInit();
	printf("\n\n------------------------------\n");
	printf("Happy Winter Season!\n");
	printf("Battery Voltage: %umV\n", batteryVoltage());
	
	// Initialise Touch Sensors
	touchInit();
	
	// Initialise LEDs
	ledInit();
	ledOn();
	
	// Initialise Timer 2 (System clock)
	T2CLKCONbits.CS = 0b0001;	// Clock source: F_OSC/4
	T2CONbits.CKPS = 0b111;		// Prescaler 1:128
	T2CONbits.OUTPS = 0b1001;	// Postscaler 1:10
	T2PR = 125;					// Compare value (16MHz/128/10/125 = 100Hz)
	PIE3bits.TMR2IE = 1;		// Enable interrupt on compare match
	T2CONbits.ON = 1;			// Enable Timer 2
	
	// Enable interrupts
	ei();

	// System clock counter (counts in units of 10ms)
	uint16_t clk = 0;
	// Program that is currently running
	uint8_t currentProgram = 0;

	// Main loop
	while(1)
	{
		// Sleep
		sleepUntilTouch();
		
		// After wake-up start in Program 0
		currentProgram = 0;
		PROGRAMS[currentProgram].initFunction();
		inputReset();
		
		// While running, perform the following tasks:
		// - Monitor touch sensors for short and long presses
		// - Monitor system clock tick flag (100Hz)
		// - After every tick, call program()
		while(1)
		{
			// Wait for system clock tick
			while(!tick);
			tick = false;
			clk++;

			// Check touch sensors every 100ms
			InputEvent events[NUM_SENSORS];
			for(uint8_t i = 0; i < NUM_SENSORS; i++) events[i] = EVENT_NONE;
			if(clk % 10 == 0)
				inputUpdate(events);
			
			// If a long press of SENSOR_FOOT_RIGHT is detected, exit inner loop
			// and go to sleep
			if(events[SENSOR_FOOT_RIGHT] == EVENT_HOLD_LONG)
				break;
			
			// Let current program do its work
			PROGRAMS[currentProgram].updateFunction(clk, events);

			// Process events that were not cleared by the program
			if(events[SENSOR_FOOT_RIGHT] == EVENT_RELEASE_SHORT || events[SENSOR_FOOT_RIGHT] == EVENT_RELEASE_LONG)
			{
				currentProgram = 0;
				printf("Switching to program \"%s\"\n", PROGRAMS[currentProgram].name);
				PROGRAMS[currentProgram].initFunction();
			}
			else if(events[SENSOR_FOOT_LEFT] == EVENT_RELEASE_SHORT || events[SENSOR_FOOT_LEFT] == EVENT_RELEASE_LONG)
			{
				currentProgram = 1;
				printf("Switching to program \"%s\"\n", PROGRAMS[currentProgram].name);
				PROGRAMS[currentProgram].initFunction();
			}
			else if(events[SENSOR_ARM_LEFT] == EVENT_RELEASE_SHORT || events[SENSOR_ARM_LEFT] == EVENT_RELEASE_LONG)
			{
				currentProgram = 2;
				printf("Switching to program \"%s\"\n", PROGRAMS[currentProgram].name);
				PROGRAMS[currentProgram].initFunction();
			}
			else if(events[SENSOR_ARM_RIGHT] == EVENT_RELEASE_SHORT || events[SENSOR_ARM_RIGHT] == EVENT_RELEASE_LONG)
			{
				currentProgram = 3;
				printf("Switching to program \"%s\"\n", PROGRAMS[currentProgram].name);
				PROGRAMS[currentProgram].initFunction();
			}
			else if(events[SENSOR_HAT] == EVENT_RELEASE_SHORT)
			{
				currentProgram = 4;
				printf("Switching to program \"%s\"\n", PROGRAMS[currentProgram].name);
				PROGRAMS[currentProgram].initFunction();
			}
			else if(events[SENSOR_HAT] == EVENT_HOLD_LONG && currentProgram != 5)
			{
				currentProgram = 5;
				printf("Switching to program \"%s\"\n", PROGRAMS[currentProgram].name);
				PROGRAMS[currentProgram].initFunction();
			}
		}
	}
}

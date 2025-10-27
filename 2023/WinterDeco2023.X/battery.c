/**
 * @file battery.c
 * @date 2024-10-06
 * @brief Implements battery.h
 */

#include<xc.h>
#include"battery.h"

#define SAMPLES 8 // 1..16

uint16_t batteryVoltage(void)
{
	// Enable FVR and wait for it to stabilise
	PMD0bits.FVRMD = 0;
	FVRCONbits.EN = 1;				// Enable FVR
	FVRCONbits.ADFVR = 0b01;		// 1.024V to FVR Buffer 1
	while(FVRCONbits.RDY == 0);
	
	// Start up ADC
	PMD2bits.ADCMD = 0;
	ADCON0bits.ON = 1;				// Enable ADC
	ADCON0bits.CONT = 0;			// No auto-trigger
	ADCON0bits.FM = 1;				// Result right aligned
	ADCON0bits.CS = 0;				// Derive ADC clock from F_OSC
	ADCON1bits.DSEN = 0;			// No double sampling
	ADCLKbits.CS = 31;				// ADC Clock freq. = F_OSC/(2*(31+1)) = 1MHz
	ADPCHbits.PCH = 0b00111110;		// Input: FVR Buffer 1
	ADREFbits.NREF = 0b0;			// Negative Reference: AVSS
	ADREFbits.PREF = 0b00;			// Positive Reference: VDD
	ADPRE = 0;						// No Precharging
	ADACQ = 32;						// Acquisition time: 32 clock cycles
	ADCAP = 0;						// No additional Sample&Hold capacity

	uint16_t adcValue = 0;
	for(uint8_t i = 0; i < SAMPLES; i++)
	{
		// Start conversion
		ADCON0bits.GO_nDONE = 1;
		// Wait for it to finish
		while(ADCON0bits.GO_nDONE);
		// Read result
		adcValue += ADRES;
	}
	adcValue /= SAMPLES;
	
	// Disable ADC
	ADCON0bits.ON = 0;
	PMD2bits.ADCMD = 1;

	// Disable FVR
	FVRCONbits.EN = 0;
	PMD0bits.FVRMD = 1;
	
	return (uint16_t)(1024ul * (1ul << 12) / adcValue);
}

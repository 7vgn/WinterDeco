/**
 * @file touch.c
 * @date 2024-10-06
 * @brief Implementation of touch.h
 */

#include<xc.h>
#include"touch.h"

/**
 * @brief Touch sensor threshold
 */
static const int16_t THRESHOLD = -1000;

/**
 * @brief Channel selections for the ADPCH register
 * @details The Sensor enum acts as an index into this array.
 */
static const uint8_t sensors[] =
{
	// TOUCH_FOOT_LEFT on RA5
	0b00000101,
	// TOUCH_FOOT_RIGHT on RB4
	0b00001100,
	// TOUCH_ARM_LEFT on RA4
	0b00000100,
	// TOUCH_ARM_RIGHT on RB5
	0b00001101,
	// TOUCH_HAT on RB6
	0b00001110
};

const char* SENSOR_NAMES[NUM_SENSORS] =
{
	"Left Foot",
	"Right Foot",
	"Left Arm",
	"Right Arm",
	"Hat"
};

void touchInit(void)
{
	// Configure sensor pins as analogue inputs
	TRISAbits.TRISA5 = 1;
	ANSELAbits.ANSELA5 = 1;
	TRISBbits.TRISB4 = 1;
	ANSELBbits.ANSELB4 = 1;
	TRISAbits.TRISA4 = 1;
	ANSELAbits.ANSELA4 = 1;
	TRISBbits.TRISB5 = 1;
	ANSELBbits.ANSELB5 = 1;
	TRISBbits.TRISB6 = 1;
	ANSELBbits.ANSELB6 = 1;

	// Configure Guard Ring Pins RC7 & RB7
	TRISCbits.TRISC7 = 0;
	TRISBbits.TRISB7 = 0;
	LATCbits.LATC7 = 0;
	LATBbits.LATB7 = 0;
	RC7PPS = 0x27; // ADGRDA
	RB7PPS = 0x28; // ADGRDB
}

bool isTouched(Sensor sensor)
{
	// Start up ADC
	PMD2bits.ADCMD = 0;
	ADCON0bits.ON = 1;				// Enable ADC
	ADCON0bits.CONT = 0;			// No auto-trigger
	ADCON0bits.FM = 1;				// Result right aligned
	ADCON0bits.CS = 0;				// Derive ADC clock from F_OSC
	ADCON1bits.DSEN = 1;			// Enable double sampling
	ADCON1bits.PPOL = 0;			// First stage precharge polarity: External VSS, internal VDD
	ADCON1bits.IPEN = 1;			// Second stage precharge polarity: inverted from first stage
	ADCON1bits.GPOL = 0;			// Guard ring starts low in first stage
	ADCON2bits.PSIS = 0;
	ADCON3bits.CALC = 0b000;		// CVD result in ADERR
	ADCLKbits.CS = 31;				// ADC Clock freq. = F_OSC/(2*(31+1)) = 1MHz
	ADPCHbits.PCH = sensors[sensor];// Input pin
	ADREFbits.NREF = 0b0;			// Negative Reference: AVSS
	ADREFbits.PREF = 0b00;			// Positive Reference: VDD
	ADPRE = 127;					// Precharging time: 127 clock cycles
	ADACQ = 127;					// Acquisition time: 127 clock cycles
	ADCAP = 0;						// No additional Sample&Hold capacity

	// First conversion: Start measurement and wait for it to finish
	ADCON0bits.GO_nDONE = 1;
	while(ADCON0bits.GO_nDONE);
	// Second conversion: Start measurement and wait for it to finish
	ADCON0bits.GO_nDONE = 1;
	while(ADCON0bits.GO_nDONE);

	// Obtain result
	int16_t result = (int16_t)ADERR;
	bool touched = result >= THRESHOLD;

	// Disable ADC
	ADCON0bits.ON = 0;
	PMD2bits.ADCMD = 1;
	
	return touched;
}

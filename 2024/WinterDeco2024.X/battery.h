/**
 * @file battery.h
 * @date 2024-10-06
 * @brief Library for checking battery voltage using the ADC and the internal
 * fixed voltage reference. 
 */

#ifndef BATTERY_H
#define	BATTERY_H

#include<stdint.h>

/**
 * @brief Reads the battery voltage
 * @details The ADC and the fixed voltage reference must not be in use. 
 * @return The battery voltage in millivolts (mV). 
 */
uint16_t batteryVoltage(void);

#endif // BATTERY_H


#ifndef SENSOR_BATTERY_H
#define SENSOR_BATTERY_H

#include <stdint.h>

// Possible states for battery level
typedef enum {
    BATTERY_OK,
    BATTERY_LOW,
    BATTERY_CRITICAL
} BatteryStatus_t;

/**
 * Convert the value read by the ADC to voltage.
 * @param adc_value ADC value (0 a 4095).
 * @return Voltage.
 */
float Battery_ConvertVoltage(uint16_t adc_value);

/**
 * Check battery health state based on read voltage.
 * @param voltage Volatge.
 * @return Battery health (BatteryStatus_t).
 */
BatteryStatus_t Battery_CheckHealth(float voltage);

#endif
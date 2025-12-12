#include "sensor_battery.h"

#define MAX_ADC_VAL 4095
#define VREF 3.3f

#define VOLTAGE_LOW_THRESHOLD 3.0f
#define VOLTAGE_CRITICAL_THRESHOLD 2.5f

float Battery_ConvertVoltage(uint16_t adc_value) {
    if (adc_value > MAX_ADC_VAL) {
        return VREF;
    }
    
    return ((float)adc_value * VREF) / MAX_ADC_VAL;
}

BatteryStatus_t Battery_CheckHealth(float voltage) {
    if (voltage < VOLTAGE_CRITICAL_THRESHOLD) {
        return BATTERY_CRITICAL;
    }
    if (voltage < VOLTAGE_LOW_THRESHOLD) {
        return BATTERY_LOW;
    }
    return BATTERY_OK;
}
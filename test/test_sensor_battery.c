#include "../unity/unity.h"
#include "../lib/sensor_battery.h"

// Unity aux functions
void setUp(void) {
}
void tearDown(void) {
}

// Test cases

void test_Battery_ConvertVoltage_MidScale(void) {
    // Read 2048 on ADC de 2048, midscale.
    uint16_t adc_mid = 2048;
    
    // Convert voltage
    float result = Battery_ConvertVoltage(adc_mid);
    
    // The result must be within (0.01V, 1.65V)
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.65f, result);
}

void test_Battery_CheckHealth_Returns_Critical(void) {
    // Voltage below the critical limit
    float voltage = 2.49f;
    
    // Check
    BatteryStatus_t status = Battery_CheckHealth(voltage);
    
    // the result must be BATTERY_CRITICAL
    TEST_ASSERT_EQUAL_INT(BATTERY_CRITICAL, status);
}

void test_Battery_CheckHealth_Returns_Low(void) {
    // Voltage must be between Low (3.0V) and Critical (2.5V)
    float voltage = 2.8f;
    
    // Check
    BatteryStatus_t status = Battery_CheckHealth(voltage);
    
    // The result must be BATTERY_LOW
    TEST_ASSERT_EQUAL_INT(BATTERY_LOW, status);
}

void test_Battery_CheckHealth_Returns_Ok(void) {
    // Voltage above Low level 
    float voltage = 3.1f;
    
    // Check
    BatteryStatus_t status = Battery_CheckHealth(voltage);
    
    // The result must be BATTERY_OK
    TEST_ASSERT_EQUAL_INT(BATTERY_OK, status);
}




int main(void) {
    // Initialize unity
    UNITY_BEGIN();
    
    // Run test cases
    RUN_TEST(test_Battery_ConvertVoltage_MidScale);
    RUN_TEST(test_Battery_CheckHealth_Returns_Critical);
    RUN_TEST(test_Battery_CheckHealth_Returns_Low);
    RUN_TEST(test_Battery_CheckHealth_Returns_Ok);
    
    // End of execution: return the results 
    return UNITY_END();
}
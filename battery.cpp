#include <Arduino.h>
#include "battery.h"
#include "cpins.h" // Include ADC configuration and battery pin definitions

// Function to read battery voltage
float readBatteryVoltage() {
  int adcValue = analogRead(BATTERY_ADC_PIN); // Read raw ADC value
  float voltage = (adcValue / ADC_MAX) * 3.3 * VOLTAGE_DIVIDER_RATIO; // Convert to voltage
  return voltage;
}

// Function to calculate battery percentage based on voltage
int calculateBatteryPercentage(float voltage) {
  float percentage = (voltage - 3.4) / (4.2 - 3.4) * 100.0; // Map voltage to 0-100%
  if (percentage < 0) {
    return 0; // Cap at 0%
  } else if (percentage > 100) {
    return 100; // Cap at 100%
  }
  return static_cast<int>(percentage);
}

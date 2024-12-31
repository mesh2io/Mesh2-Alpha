#ifndef CPINS_H
#define CPINS_H

#include <Arduino.h> // Required for GPIO definitions and Arduino-specific types

// Define pins and other hardware-specific configurations
#define SPEAKER_PIN 25
#define TFCARD_CS_PIN 39
#define TOUCH_INT_PIN 16
#define RADIO_CS_PIN 9

// LoRa Pins
#define LORA_SCK_PIN 40
#define LORA_MISO_PIN 38
#define LORA_MOSI_PIN 41
#define LORA_DIO1_PIN 45
#define LORA_RESET_PIN 17
#define LORA_BUSY_PIN 13
#define LORA_CS_PIN 9

// ADC Configuration
#define ADC_MAX 4095.0                // Maximum ADC value for ESP32
#define VOLTAGE_DIVIDER_RATIO 2.0     // Voltage divider ratio
#define ADC_ATTENUATION ADC_11db      // ADC attenuation for full voltage range

// Trackball Pins
#define TRACKBALL_UP_PIN 3
#define TRACKBALL_DOWN_PIN 15
#define TRACKBALL_BUTTON_PIN 0

// ADC Pin for Battery Voltage
#define BATTERY_ADC_PIN 4

// SPI Keyboard Pins
#define KEYBOARD_POWERON 10
#define KEYBOARD_SDA_PIN 18  // SPI Data (MOSI)
#define KEYBOARD_SCL_PIN 8   // SPI Clock
#define KEYBOARD_INT_PIN 46  // Interrupt Pin



extern const char *SETTINGS_FILE;
extern const char *MESSAGES_FILE;
const int MAX_MESSAGES = 20;
const int MESSAGE_SIZE = 128;


#endif // CPINS_H

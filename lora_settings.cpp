#include <Arduino.h>
#include "lora_settings.h"
#include <LoRa.h> // Include the LoRa library
#include "TDeckConfig.h" // Include the display configuration

extern LGFX_S3TDeck display;

void applyLoRaSettings() {
  // Configure SPI pins
  SPI.begin(LORA_SCK_PIN, LORA_MISO_PIN, LORA_MOSI_PIN, LORA_CS_PIN);

  // Ensure the display is inactive
  digitalWrite(GPIO_NUM_12, HIGH); // GPIO_NUM_12 is the CS pin for the display

  // Assign LoRa module pins
  LoRa.setPins(LORA_CS_PIN, LORA_RESET_PIN, LORA_BUSY_PIN);

  display.setCursor(10, 90); // Set the cursor position below SD card initialization
  display.setTextSize(1);

  // Attempt to initialize LoRa
  if (!LoRa.begin(frequency)) { // Start LoRa with the specified frequency
    display.setTextColor(TFT_RED);
    display.println("LoRa Init Failed!");
    display.display(); // Update the display
    Serial.println("LoRa initialization failed!");
    while (1); // Halt the program on failure
  }

  // Apply LoRa settings
  LoRa.setSpreadingFactor(spreadingFactor);
  LoRa.setSignalBandwidth(bandwidth);
  LoRa.setCodingRate4(codingRate);

  // Reactivate the display
  digitalWrite(GPIO_NUM_12, LOW);

  display.setTextColor(TFT_GREEN);
  display.println("LoRa Initialized!");
  display.display(); // Update the display
  Serial.println("LoRa settings applied successfully.");
}

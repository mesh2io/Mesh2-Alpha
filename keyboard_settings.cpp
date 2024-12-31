#include <Arduino.h>
#include "keyboard_settings.h"
#include "cpins.h"
#include "TDeckConfig.h"
#include "storage.h"
#include "lora_settings.h"
#include <LovyanGFX.hpp>

extern LGFX_S3TDeck display; // Use the display object from TDeckConfig.h

void toggleKeyboardBacklight() {
    Wire.beginTransmission(KEYBOARD_I2C_ADDRESS);
    Wire.write(0xB0); // Example: Assuming 0xB0 represents the Alt key
    Wire.write(0x42); // Example: Assuming 0x42 represents the 'B' key
    Wire.endTransmission();
    delay(100); // Short delay for the keyboard to process the input
    Serial.println("Keyboard backlight toggled.");
}

void initializeKeyboard() {
  // Power on the keyboard
  pinMode(KEYBOARD_POWERON, OUTPUT);
  digitalWrite(KEYBOARD_POWERON, HIGH); // Turn on the keyboard power
  delay(100); // Allow time for the keyboard to power up

  // Initialize I2C communication
  Wire.begin(KEYBOARD_SDA_PIN, KEYBOARD_SCL_PIN);
  Wire.setClock(400000); // Set I2C clock to 400kHz
  Serial.println("Keyboard initialized.");
  toggleKeyboardBacklight();
}

char readKey() {
  char keyData = '\0';

  Wire.requestFrom(KEYBOARD_I2C_ADDRESS, 1); // Request 1 byte from the keyboard
  if (Wire.available()) {
    keyData = Wire.read();
    if (keyData != '\0') { // Only process if a key is pressed
      Serial.print("Key Pressed (ASCII): ");
      Serial.println((int)keyData); // Print the ASCII value of the key
    }
  }

  return keyData; // Return the key or '\0' if no key was pressed
}
String getKeyboardInput(const char *prompt) {
  String input = "";
  String lastInput = ""; // Store the last input to detect changes

  Serial.println(prompt);

  // Display the prompt on the screen
  extern LGFX_S3TDeck display;
  display.fillRect(0, 30, 320, 200, TFT_BLACK);
  display.setCursor(10, 50);
  display.setTextColor(TFT_WHITE);
  display.print(prompt);
  display.display();

  while (true) {
    char key = readKey();
    if (key == '\r') { // Handle Carriage Return (Enter key)
      break;
    } else if (key == '\b' && input.length() > 0) { // Handle Backspace
      input.remove(input.length() - 1);
    } else if (key != '\0' && key != '\r' && key != '\b') { // Append valid key
      input += key;
    }

    // Update the display only if the input has changed
    if (input != lastInput) {
      lastInput = input;
      display.fillRect(10, 80, 300, 20, TFT_BLACK); // Clear input area
      display.setCursor(10, 80);
      display.setTextColor(TFT_WHITE);
      display.print(input);
      display.display();
    }
  }

  Serial.println("Input received: " + input); // Debugging output
  return input;
}



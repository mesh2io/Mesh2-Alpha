#ifndef KEYBOARD_SETTINGS_H
#define KEYBOARD_SETTINGS_H

#include <Wire.h>

// Define I2C address for the keyboard
#define KEYBOARD_I2C_ADDRESS 0x55 // Verify this address with an I2C scanner if unsure

// Function declarations
void initializeKeyboard();              // Initialize the keyboard
String getKeyboardInput(const char *prompt); // Get input string from the keyboard
char readKey();                         // Read a single key press

#endif // KEYBOARD_SETTINGS_H

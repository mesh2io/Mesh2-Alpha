#include <Arduino.h>
#include "menu.h"
#include "send_message.h"
#include "TDeckConfig.h"
#include "lora_settings.h"
#include "storage.h"
#include "wifi_settings.h"
#include "keyboard_settings.h"
#include <LovyanGFX.hpp>
#include "display_helper.h"

extern LGFX_S3TDeck display; // Use the display object

// Menu data
const char *mainMenuItems[] = {
    "Configure",
    "Send Message",
    "Read Messages",
    "Option 1",
    "Option 2",
    "Option 3"};
int mainMenuItemCount = sizeof(mainMenuItems) / sizeof(mainMenuItems[0]);

const char *configureMenuItems[] = {
    "Set Call-sign",
    "Set Frequency",
    "Set Spreading Factor",
    "Set Bandwidth",
    "Set Coding Rate",
    "Set WiFi SSID",
    "Set WiFi Password",
    "Exit"};
int configureMenuItemCount = sizeof(configureMenuItems) / sizeof(configureMenuItems[0]);

const char *spreadingFactorOptions[] = {"7", "8", "9", "10", "11", "12"};
int spreadingFactorOptionCount = sizeof(spreadingFactorOptions) / sizeof(spreadingFactorOptions[0]);

const char *codingRateOptions[] = {"4/5", "4/6", "4/7", "4/8"};
int codingRateOptionCount = sizeof(codingRateOptions) / sizeof(codingRateOptions[0]);

const char *bandwidthOptions[] = {
    "7.8kHz", "10.4kHz", "15.6kHz", "20.8kHz", "31.25kHz",
    "41.7kHz", "62.5kHz", "125kHz", "250kHz", "500kHz"};
float bandwidthValues[] = {
    7.8E3, 10.4E3, 15.6E3, 20.8E3, 31.25E3, 41.7E3, 62.5E3, 125E3, 250E3, 500E3};
int bandwidthOptionCount = sizeof(bandwidthOptions) / sizeof(bandwidthOptions[0]);

int currentMenuIndex = 0;
int currentSubMenuIndex = 0;
bool inSubMenu = false;

void waitForTrackballRelease() {
    while (digitalRead(TRACKBALL_BUTTON_PIN) == LOW) {
        delay(10); // Wait until the trackball button is released
    }
}

void drawMainMenu() {
    display.fillRect(0, 30, 320, 200, TFT_BLACK); // Clear menu area
    for (int i = 0; i < mainMenuItemCount; i++) {
        display.setCursor(10, 50 + (i * 20));
        display.setTextColor(i == currentMenuIndex ? TFT_GREEN : TFT_WHITE);
        display.println(mainMenuItems[i]);
    }
    display.display(); // Update the display
}

void drawConfigureMenu() {
    display.fillRect(0, 30, 320, 200, TFT_BLACK); // Clear menu area
    for (int i = 0; i < configureMenuItemCount; i++) {
        display.setCursor(10, 50 + (i * 20));
        display.setTextColor(i == currentSubMenuIndex ? TFT_GREEN : TFT_WHITE);
        display.println(configureMenuItems[i]);
    }
    display.display(); // Update the display
}

void handleFrequencyInput() {
    String frequencyInput = getKeyboardInput("Enter Frequency (MHz):");
    frequency = frequencyInput.toFloat() * 1000000; // Convert MHz to Hz
    saveConfiguration();
    displayMessage("Frequency Updated");
}
void handleCodingRateSelection() {
    currentSubMenuIndex = 0; // Reset index for coding rate options

    while (true) {
        // Clear menu area and display the coding rate options
        display.fillRect(0, 30, 320, 200, TFT_BLACK);
        for (int i = 0; i < codingRateOptionCount; i++) {
            display.setCursor(10, 50 + (i * 20));
            display.setTextColor(i == currentSubMenuIndex ? TFT_GREEN : TFT_WHITE);
            display.println(codingRateOptions[i]);
        }
        display.display();

        // Handle trackball input for navigation and selection
        static bool lastUpState = HIGH, lastDownState = HIGH, lastButtonState = HIGH;
        bool currentUpState = digitalRead(TRACKBALL_UP_PIN);
        bool currentDownState = digitalRead(TRACKBALL_DOWN_PIN);
        bool currentButtonState = digitalRead(TRACKBALL_BUTTON_PIN);

        if (currentUpState == LOW && lastUpState == HIGH) {
            currentSubMenuIndex = (currentSubMenuIndex == 0) ? codingRateOptionCount - 1 : currentSubMenuIndex - 1;
        }

        if (currentDownState == LOW && lastDownState == HIGH) {
            currentSubMenuIndex = (currentSubMenuIndex + 1) % codingRateOptionCount;
        }

        if (currentButtonState == LOW && lastButtonState == HIGH) {
            waitForTrackballRelease(); // Ensure the button is released
            codingRate = currentSubMenuIndex + 5; // Map index to coding rate (e.g., 4/5 -> 5, 4/6 -> 6)
            saveConfiguration(); // Save the new coding rate to configuration
            displayMessage("Coding Rate Updated"); // Notify the user
            break; // Exit the loop after selection
        }

        // Update the button states for debouncing
        lastUpState = currentUpState;
        lastDownState = currentDownState;
        lastButtonState = currentButtonState;

        delay(100); // Prevent rapid cycling
    }
}

void handleSpreadingFactorSelection() {
    currentSubMenuIndex = 0;
    while (true) {
        display.fillRect(0, 30, 320, 200, TFT_BLACK);
        for (int i = 0; i < spreadingFactorOptionCount; i++) {
            display.setCursor(10, 50 + (i * 20));
            display.setTextColor(i == currentSubMenuIndex ? TFT_GREEN : TFT_WHITE);
            display.println(spreadingFactorOptions[i]);
        }
        display.display();

        static bool lastUpState = HIGH, lastDownState = HIGH, lastButtonState = HIGH;
        bool currentUpState = digitalRead(TRACKBALL_UP_PIN);
        bool currentDownState = digitalRead(TRACKBALL_DOWN_PIN);
        bool currentButtonState = digitalRead(TRACKBALL_BUTTON_PIN);

        if (currentUpState == LOW && lastUpState == HIGH) {
            currentSubMenuIndex = (currentSubMenuIndex == 0) ? spreadingFactorOptionCount - 1 : currentSubMenuIndex - 1;
        }

        if (currentDownState == LOW && lastDownState == HIGH) {
            currentSubMenuIndex = (currentSubMenuIndex + 1) % spreadingFactorOptionCount;
        }

        if (currentButtonState == LOW && lastButtonState == HIGH) {
            waitForTrackballRelease();
            spreadingFactor = atoi(spreadingFactorOptions[currentSubMenuIndex]);
            saveConfiguration();
            displayMessage("Spreading Factor Updated");
            break;
        }

        lastUpState = currentUpState;
        lastDownState = currentDownState;
        lastButtonState = currentButtonState;

        delay(100);
    }
}

void handleBandwidthSelection() {
    currentSubMenuIndex = 0;
    while (true) {
        display.fillRect(0, 30, 320, 200, TFT_BLACK);
        for (int i = 0; i < bandwidthOptionCount; i++) {
            display.setCursor(10, 50 + (i * 20));
            display.setTextColor(i == currentSubMenuIndex ? TFT_GREEN : TFT_WHITE);
            display.println(bandwidthOptions[i]);
        }
        display.display();

        static bool lastUpState = HIGH, lastDownState = HIGH, lastButtonState = HIGH;
        bool currentUpState = digitalRead(TRACKBALL_UP_PIN);
        bool currentDownState = digitalRead(TRACKBALL_DOWN_PIN);
        bool currentButtonState = digitalRead(TRACKBALL_BUTTON_PIN);

        if (currentUpState == LOW && lastUpState == HIGH) {
            currentSubMenuIndex = (currentSubMenuIndex == 0) ? bandwidthOptionCount - 1 : currentSubMenuIndex - 1;
        }

        if (currentDownState == LOW && lastDownState == HIGH) {
            currentSubMenuIndex = (currentSubMenuIndex + 1) % bandwidthOptionCount;
        }

        if (currentButtonState == LOW && lastButtonState == HIGH) {
            waitForTrackballRelease();
            bandwidth = bandwidthValues[currentSubMenuIndex];
            saveConfiguration();
            displayMessage("Bandwidth Updated");
            break;
        }

        lastUpState = currentUpState;
        lastDownState = currentDownState;
        lastButtonState = currentButtonState;

        delay(100);
    }
}

void handleMenuInput() {
    static bool lastUpState = HIGH, lastDownState = HIGH, lastButtonState = HIGH;

    bool currentUpState = digitalRead(TRACKBALL_UP_PIN);
    bool currentDownState = digitalRead(TRACKBALL_DOWN_PIN);
    bool currentButtonState = digitalRead(TRACKBALL_BUTTON_PIN);

    if (currentUpState == LOW && lastUpState == HIGH) {
        if (inSubMenu) {
            // Properly handle wrapping in the submenu
            currentSubMenuIndex = (currentSubMenuIndex == 0) ? configureMenuItemCount - 1 : currentSubMenuIndex - 1;
            drawConfigureMenu();
        } else {
            // Properly handle wrapping in the main menu
            currentMenuIndex = (currentMenuIndex == 0) ? mainMenuItemCount - 1 : currentMenuIndex - 1;
            drawMainMenu();
        }
    }

    if (currentDownState == LOW && lastDownState == HIGH) {
        if (inSubMenu) {
            // Properly handle wrapping in the submenu
            currentSubMenuIndex = (currentSubMenuIndex + 1) % configureMenuItemCount;
            drawConfigureMenu();
        } else {
            // Properly handle wrapping in the main menu
            currentMenuIndex = (currentMenuIndex + 1) % mainMenuItemCount;
            drawMainMenu();
        }
    }

    if (currentButtonState == LOW && lastButtonState == HIGH) {
        waitForTrackballRelease();
        selectMenuItem();
    }

    lastUpState = currentUpState;
    lastDownState = currentDownState;
    lastButtonState = currentButtonState;
}

void selectMenuItem() {
    if (!inSubMenu) {
        switch (currentMenuIndex) {
            case 0: // Configure
                inSubMenu = true;
                currentSubMenuIndex = 0;
                drawConfigureMenu();
                break;
            case 1: // Send Message
                handleSendMessage();
                break;
            default:
                displayMessage("Option unavailable.");
                delay(2000);
                drawMainMenu();
                break;
        }
    } else {
        switch (currentSubMenuIndex) {
            case 0: // Set Call-sign
                callsign = getKeyboardInput("Enter Call-sign:");
                saveConfiguration();
                displayMessage("Call-sign Updated");
                break;
            case 1: // Set Frequency
                handleFrequencyInput();
                break;
            case 2: // Set Spreading Factor
                handleSpreadingFactorSelection();
                break;
            case 3: // Set Bandwidth
                handleBandwidthSelection();
                break;
            case 4: // Set Coding Rate
                handleCodingRateSelection();
                break;
            case 5: // Set WiFi SSID
                setWiFiSSID(getKeyboardInput("Enter WiFi SSID:"));
                displayMessage("WiFi SSID Updated");
                break;
            case 6: // Set WiFi Password
                setWiFiPassword(getKeyboardInput("Enter WiFi Password:"));
                displayMessage("WiFi Password Updated");
                break;
            case 7: // Exit
                inSubMenu = false; // Exit the submenu
                drawMainMenu();
                return;
            default:
                displayMessage("Submenu unavailable.");
                delay(2000);
                drawMainMenu();
                break;
        }

        // If still in the submenu after handling, redraw the configuration menu
        if (inSubMenu) {
            drawConfigureMenu();
        }
    }
}


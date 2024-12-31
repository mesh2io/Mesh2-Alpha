#include "statusbar.h"
#include "battery.h"
#include "TDeckConfig.h" // Ensure this is included for display object
#include "wifi_settings.h" // Include Wi-Fi functionality
#include "storage.h" // For message-related status
#include <LovyanGFX.hpp>

extern LGFX_S3TDeck display;

static unsigned long lastUpdateTime = 0;
const unsigned long updateInterval = 1000; // Update every 1 second
static bool hasNewMessage = false; // Tracks if there's a new message
static bool messageIndicatorFlash = false; // For flashing the "New Message" text

void setNewMessageIndicator(bool hasMessage) {
    hasNewMessage = hasMessage;
    updateStatusBar(); // Update the status bar to reflect the indicator
}

void clearNewMessageFlag() {
  setNewMessageIndicator(false); // Clear the new message flag
    hasNewMessage = false;
    updateStatusBar();
}


void updateStatusBar() {
    unsigned long currentTime = millis();

    if (currentTime - lastUpdateTime >= updateInterval) {
        lastUpdateTime = currentTime;

        // Clear the top bar area
        display.fillRect(0, 0, 320, 30, TFT_BLACK);

        // Display callsign
        display.setCursor(0, 0);
        display.setTextColor(TFT_WHITE);
        display.print(callsign);

        // Display frequency
        display.setCursor(0, 10);
        display.setTextColor(TFT_WHITE);
        display.printf("%.3f MHz", frequency / 1000000.0);

        display.setCursor(240, 10);
if (hasNewMessage) {
    display.setTextColor(millis() % 1000 < 500 ? TFT_GREEN : TFT_BLACK); // Blinking
    display.print("New Message");
} else {
    display.setTextColor(TFT_WHITE);
    display.print("No new messages");
}

        // Display Wi-Fi status
        display.setCursor(110, 0);
        if (isWiFiConnected()) {
            display.setTextColor(TFT_BLUE);
            display.print("Wi-Fi Connected");
        } else {
            display.setTextColor(TFT_RED);
            display.print("Wi-Fi Disconnected");
        }

        // Display battery voltage and percentage
        float batteryVoltage = readBatteryVoltage();
        int batteryPercentage = calculateBatteryPercentage(batteryVoltage);

        display.setCursor(240, 0);
        display.setTextColor(TFT_WHITE);
        display.printf("%.2fV", batteryVoltage);

        display.setCursor(280, 0);
        if (batteryPercentage > 30) {
            display.setTextColor(TFT_GREEN);
        } else if (batteryPercentage >= 20) {
            display.setTextColor(TFT_YELLOW);
        } else {
            display.setTextColor(TFT_RED);
        }
        display.printf("(%d%%)", batteryPercentage);


        // Apply the updates
        display.display();
    }
}

#include "TDeckConfig.h"
#include "cpins.h"
#include <LovyanGFX.hpp>
#include <LoRa.h>
#include <SPI.h>
#include "keyboard_settings.h"
#include "defaults.h"
#include "menu.h"
#include "lora_settings.h"
#include "storage.h"
#include "statusbar.h"
#include "wifi_settings.h"
#include "send_message.h"
#include "lora_user_list.h"
#include "message_receive.h"

extern LGFX_S3TDeck display;

// Timing variables for non-blocking updates
unsigned long lastStatusBarUpdate = 0;
const unsigned long statusBarUpdateInterval = 1000; // Update status bar every 1 second

unsigned long lastUserListFetch = 0;
const unsigned long userListFetchInterval = 60000; // Fetch user list every 1 minute

void setup() {
    delay(3000); // Wait for Serial initialization
    Serial.begin(115200); // Initialize Serial for debugging

    // Initialize the display
    display.init();
    display.setRotation(1);
    display.fillScreen(TFT_BLACK);

    // Display setup status
    display.setCursor(10, 10);
    display.setTextColor(TFT_WHITE);
    display.println("Initializing...");

    // Set up trackball pins
    pinMode(TRACKBALL_UP_PIN, INPUT_PULLUP);
    pinMode(TRACKBALL_DOWN_PIN, INPUT_PULLUP);
    pinMode(TRACKBALL_BUTTON_PIN, INPUT_PULLUP);

    // Initialize the keyboard
    initializeKeyboard();

    // Initialize the SD card and load configuration
    if (!initializeSD()) {
        display.setCursor(10, 50);
        display.setTextColor(TFT_RED);
        display.println("SD Card Init Failed!");
        Serial.println("SD Card initialization failed. Continuing with defaults.");
    } else {
        loadConfiguration(); // Load configuration from the SD card
        display.setCursor(10, 50);
        display.setTextColor(TFT_GREEN);
        display.println("SD Card Initialized!");
    }

    // Apply LoRa settings
    applyLoRaSettings();
    display.setCursor(10, 70);
    display.setTextColor(TFT_GREEN);
    display.println("LoRa Settings Applied");
    delay(2000);

    // Clear setup status
    display.fillScreen(TFT_BLACK);

    // Initialize WiFi
    initializeWiFi();

    // Fetch initial user list
    fetchLoRaUserList();

    // Draw the initial menu and status bar
    drawMainMenu();
    updateStatusBar();
}

void loop() {
    unsigned long currentTime = millis();

    // Check for new LoRa messages
    receiveLoRaMessages();

    // Handle menu input
    handleMenuInput();

    // Update the status bar at a defined interval
    if (currentTime - lastStatusBarUpdate >= statusBarUpdateInterval) {
        lastStatusBarUpdate = currentTime;
        updateStatusBar();
    }

    // Fetch user list at a defined interval if WiFi is connected
    if (isWiFiConnected() && currentTime - lastUserListFetch >= userListFetchInterval) {
        lastUserListFetch = currentTime;
        fetchLoRaUserList(); // Uses cached data if fetching fails
    }
}

// Process received LoRa messages
void processReceivedMessage(const String &message) {
    // Check if the message is for this device
    if (message.startsWith("MSG," + callsign + ",")) {
        // Extract the actual message content
        String extractedMessage = message.substring(("MSG," + callsign + ",").length());

        // Save the message to the SD card
        saveMessageToSD(extractedMessage);

        // Update the status bar with a new message notification
        setNewMessageIndicator(true);
    }
}

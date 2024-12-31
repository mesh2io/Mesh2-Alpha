#include "send_message.h"
#include "lora_user_list.h" // Include the new file for user list management
#include "TDeckConfig.h"
#include "keyboard_settings.h"
#include "storage.h"
#include <LovyanGFX.hpp>
#include <LoRa.h>
#include "cpins.h"
#include "lora_settings.h"
#include "lora_helper.h"
#include "menu.h"
#include "display_helper.h"

extern LGFX_S3TDeck display;



// Function to handle sending a LoRa message
void handleSendMessage() {
    String userList = getLoRaUserList();
    if (userList.isEmpty()) {
        displayMessage("No users available.");
        return;
    }

    // Parse usernames from the list
    String users[10]; // Adjust the size based on the expected user count
    int userCount = 0;

    int startIdx = 0;
    for (int i = 0; i < userList.length(); i++) {
        if (userList[i] == ',') {
            users[userCount++] = userList.substring(startIdx, i);
            startIdx = i + 1;
        }
    }
    if (startIdx < userList.length()) {
        users[userCount++] = userList.substring(startIdx);
    }

    int currentUserIndex = 0;
    String message = "";

    while (true) {
        // Display the user selection menu
        display.fillRect(0, 30, 320, 200, TFT_BLACK); // Clear the menu area
        display.setCursor(10, 50);
        display.setTextColor(TFT_WHITE);
        display.println("Select User:");
        for (int i = 0; i < userCount; i++) {
            if (i == currentUserIndex) {
                display.setTextColor(TFT_GREEN);
            } else {
                display.setTextColor(TFT_WHITE);
            }
            display.println(users[i]);
        }
        display.setTextColor(TFT_WHITE);
        display.setCursor(10, 150);
        display.println("Press Enter to select");
        display.display();

        static bool lastUpState = HIGH;
        static bool lastDownState = HIGH;
        static bool lastButtonState = HIGH;

        bool currentUpState = digitalRead(TRACKBALL_UP_PIN);
        bool currentDownState = digitalRead(TRACKBALL_DOWN_PIN);
        bool currentButtonState = digitalRead(TRACKBALL_BUTTON_PIN);

        if (currentUpState == LOW && lastUpState == HIGH) {
            currentUserIndex = (currentUserIndex == 0) ? userCount - 1 : currentUserIndex - 1;
        }

        if (currentDownState == LOW && lastDownState == HIGH) {
            currentUserIndex = (currentUserIndex + 1) % userCount;
        }

        if (currentButtonState == LOW && lastButtonState == HIGH) {
            waitForTrackballRelease(); // Ensure the trackball button is released
            message = getKeyboardInput("Enter Message:");
            sendLoRaMessage(users[currentUserIndex], message); // Use the correct function signature
            displayMessage("Message Sent");
            return;
        }

        lastUpState = currentUpState;
        lastDownState = currentDownState;
        lastButtonState = currentButtonState;

        delay(100); // Prevent rapid cycling
    }
}
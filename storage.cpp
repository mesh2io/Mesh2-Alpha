#include <SD.h>
#include <Arduino.h>
#include "FS.h"
#include "storage.h"
#include "defaults.h"
#include "cpins.h"
#include "lora_user_list.h"

// Initialize the SD card
bool initializeSD() {
    Serial.println("Initializing SD card...");
    if (!SD.begin(TFCARD_CS_PIN)) {
        Serial.println("Failed to initialize SD card!");
        return false;
    }
    Serial.println("SD card initialized successfully.");
    return true;
}

// Load configuration from the SD card
void loadConfiguration() {
    if (!SD.exists("/config.txt")) {
        Serial.println("Configuration file not found. Using defaults.");
        return;
    }

    File configFile = SD.open("/config.txt", FILE_READ);
    if (!configFile) {
        Serial.println("Failed to open configuration file for reading.");
        return;
    }

    callsign = configFile.readStringUntil('\n');
    frequency = configFile.readStringUntil('\n').toInt();
    spreadingFactor = configFile.readStringUntil('\n').toInt();
    bandwidth = configFile.readStringUntil('\n').toInt();
    codingRate = configFile.readStringUntil('\n').toInt();
    callsign.trim();
    configFile.close();

    Serial.println("Configuration loaded successfully.");
}

// Save configuration to the SD card
void saveConfiguration() {
    File configFile = SD.open("/config.txt", FILE_WRITE);
    if (!configFile) {
        Serial.println("Failed to open configuration file for writing.");
        return;
    }

    configFile.println(callsign);
    configFile.println(frequency);
    configFile.println(spreadingFactor);
    configFile.println(bandwidth);
    configFile.println(codingRate);
    configFile.close();

    Serial.println("Configuration saved successfully.");
}

// Read WiFi SSID
String readWiFiSSID() {
    File file = SD.open("/wifi_ssid.txt", FILE_READ);
    if (!file) {
        Serial.println("Failed to open WiFi SSID file for reading.");
        return "";
    }
    String ssid = file.readStringUntil('\n');
    ssid.trim(); // Remove extra spaces or newlines
    file.close();
    return ssid;
}

// Read WiFi Password
String readWiFiPassword() {
    File file = SD.open("/wifi_password.txt", FILE_READ);
    if (!file) {
        Serial.println("Failed to open WiFi password file for reading.");
        return "";
    }
    String password = file.readStringUntil('\n');
    password.trim();
    file.close();
    return password;
}

// Save WiFi credentials
void saveWiFiCredentials(const String &ssid, const String &password) {
    // Save SSID
    File ssidFile = SD.open("/wifi_ssid.txt", FILE_WRITE);
    if (!ssidFile) {
        Serial.println("Failed to open WiFi SSID file for writing.");
        return;
    }
    ssidFile.println(ssid);
    ssidFile.close();

    // Save Password
    File passwordFile = SD.open("/wifi_password.txt", FILE_WRITE);
    if (!passwordFile) {
        Serial.println("Failed to open WiFi password file for writing.");
        return;
    }
    passwordFile.println(password);
    passwordFile.close();

    Serial.println("WiFi credentials saved successfully.");
}

void saveLoRaUserList(const String &userList) {
    File file = SD.open("/lora_users.txt", FILE_WRITE);
    if (!file) {
        Serial.println("Failed to open LoRa user list file for writing.");
        return;
    }
    file.println(userList);
    file.close();
    Serial.println("LoRa user list saved successfully.");
}

String readLoRaUserList() {
    if (!SD.exists("/lora_users.txt")) {
        Serial.println("LoRa user list not found on SD card.");
        return "";
    }

    File file = SD.open("/lora_users.txt", FILE_READ);
    if (!file) {
        Serial.println("Failed to open LoRa user list file for reading.");
        return "";
    }

    String userList = file.readString();
    file.close();
    return userList;
}

void saveMessageToSD(const String &message) {
    File file = SD.open("/messages.txt", FILE_APPEND);
    if (file) {
        file.println(message);
        file.close();
        Serial.println("Message saved to SD card.");
    } else {
        Serial.println("Failed to save message to SD card.");
    }
}

String readStoredMessages() {
    File file = SD.open("/messages.txt", FILE_READ);
    if (!file) {
        return "No messages.";
    }
    String messages = file.readString();
    file.close();
    return messages;
}
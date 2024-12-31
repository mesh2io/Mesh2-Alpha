#include "wifi_settings.h"
#include "storage.h"
#include <WiFi.h>

String ssid = "";
String password = "";

// Initialize WiFi
void initializeWiFi() {
    ssid = getWiFiSSID();
    password = getWiFiPassword();
    connectToWiFi();
}

// Connect to WiFi
void connectToWiFi() {
    if (ssid.isEmpty() || password.isEmpty()) {
        Serial.println("WiFi credentials not set.");
        return;
    }
    WiFi.begin(ssid.c_str(), password.c_str());
    Serial.println("Connecting to WiFi...");
}

// Check WiFi connection status
bool isWiFiConnected() {
    return WiFi.status() == WL_CONNECTED;
}

// Get stored SSID
String getWiFiSSID() {
    return readWiFiSSID(); // Reads from SD card
}

// Get stored Password
String getWiFiPassword() {
    return readWiFiPassword(); // Reads from SD card
}

// Set new SSID
void setWiFiSSID(const String &newSSID) {
    ssid = newSSID;
    saveWiFiCredentials(ssid, password);
}

// Set new Password
void setWiFiPassword(const String &newPassword) {
    password = newPassword;
    saveWiFiCredentials(ssid, password);
}

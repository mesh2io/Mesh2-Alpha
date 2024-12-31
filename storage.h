#include <Arduino.h>

#ifndef STORAGE_H
#define STORAGE_H

bool initializeSD(); // Changed to bool to match the implementation
void loadConfiguration();
void saveConfiguration();
void saveWiFiCredentials(const String &ssid, const String &password);
String readWiFiSSID(); // Reads WiFi SSID from persistent storage
String readWiFiPassword(); // Reads WiFi password from persistent storage
String readLoRaUserList();
void saveMessage(const String &message);
void saveMessageToSD(const String &message); // Add this declaration
String readStoredMessages();

#endif // STORAGE_H

#ifndef WIFI_SETTINGS_H
#define WIFI_SETTINGS_H

#include <WiFi.h>

// Function declarations
void initializeWiFi();
void connectToWiFi();
bool isWiFiConnected();
String getWiFiSSID();
String getWiFiPassword();
void setWiFiSSID(const String &ssid);
void setWiFiPassword(const String &password);
void saveWiFiCredentials(const String &ssid, const String &password);

#endif // WIFI_SETTINGS_H

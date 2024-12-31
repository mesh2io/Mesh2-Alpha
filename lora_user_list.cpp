#include "lora_user_list.h"
#include "storage.h"
#include "wifi_settings.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <SD.h>
#include <Arduino.h>
#include <WiFiClient.h>

String extractedUsernames = "";

// Function to fetch and process LoRa user list
void fetchLoRaUserList() {
    if (!isWiFiConnected()) {
        Serial.println("WiFi is not connected. Cannot fetch user list.");
        return;
    }

    HTTPClient http;
    http.begin("https://s2.slynx.net/lora_users.php"); // Replace with your actual API URL
    int httpResponseCode = http.GET();

    if (httpResponseCode == 200) {
        String payload = http.getString();
        saveLoRaUserList(payload); // Save the raw JSON to the SD card for caching
        Serial.println("LoRa user list downloaded successfully.");

        // Parse JSON to extract usernames
        DynamicJsonDocument doc(1024);
        DeserializationError error = deserializeJson(doc, payload);

        if (!error) {
            extractedUsernames = "";
            for (JsonObject obj : doc.as<JsonArray>()) {
                if (obj.containsKey("username")) {
                    extractedUsernames += obj["username"].as<String>() + ",";
                }
            }

            if (extractedUsernames.endsWith(",")) {
                extractedUsernames.remove(extractedUsernames.length() - 1); // Remove trailing comma
            }
        } else {
            Serial.println("Failed to parse JSON. Using cached data.");
        }
    } else {
        Serial.println("Failed to fetch LoRa user list. Using cached data.");
    }

    http.end();
}

// Cached usernames

String getLoRaUserList() {
    if (!extractedUsernames.isEmpty()) {
        return extractedUsernames;
    }

    String rawList = readLoRaUserList(); // Use the function from storage.cpp
    if (rawList.isEmpty()) {
        return "";
    }

    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, rawList);
    if (error) {
        Serial.println("Failed to parse LoRa user list JSON.");
        return "";
    }

    extractedUsernames = "";
    for (JsonObject user : doc.as<JsonArray>()) {
        if (!extractedUsernames.isEmpty()) {
            extractedUsernames += ",";
        }
        extractedUsernames += user["username"].as<String>();
    }

    return extractedUsernames;
}
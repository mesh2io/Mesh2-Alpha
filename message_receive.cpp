#include "message_receive.h"
#include "lora_settings.h"
#include "storage.h"
#include <LoRa.h>
#include <Arduino.h>

// Global flag for new messages
bool hasNewMessage = false;

void receiveLoRaMessages() {
    int packetSize = LoRa.parsePacket();
    if (packetSize > 0) {
        String receivedMessage = "";
        while (LoRa.available()) {
            receivedMessage += (char)LoRa.read();
        }

        // Parse the message: MSG,Username,Message
        int firstComma = receivedMessage.indexOf(',');
        int secondComma = receivedMessage.indexOf(',', firstComma + 1);

        if (firstComma == -1 || secondComma == -1) {
            Serial.println("Invalid message format.");
            return;
        }

        String type = receivedMessage.substring(0, firstComma);
        String recipient = receivedMessage.substring(firstComma + 1, secondComma);
        String content = receivedMessage.substring(secondComma + 1);

        if (type == "MSG" && recipient == callsign) {
            saveMessage(content);
            hasNewMessage = true;
            Serial.println("New message received: " + content);
        }
    }
}

bool checkNewMessage() {
    return hasNewMessage;
}

void clearNewMessageFlag() {
    hasNewMessage = false;
}

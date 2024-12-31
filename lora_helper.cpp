#include "lora_helper.h"
#include <LoRa.h>
#include <Arduino.h>
#include "display_helper.h" // For displaying messages
#include "storage.h" // For displaying messages

extern LGFX_S3TDeck display; // Use the global display object

void sendLoRaMessage(const String &recipient, const String &message) {
    String fullMessage = "MSG," + recipient + "," + message;
    LoRa.beginPacket();
    LoRa.print(fullMessage);
    LoRa.endPacket();
    Serial.println("LoRa message sent: " + fullMessage);
}
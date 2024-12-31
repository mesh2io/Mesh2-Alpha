#ifndef LORA_HELPER_H
#define LORA_HELPER_H

#include "TDeckConfig.h"
#include "lora_settings.h"
#include "cpins.h"
#include <Arduino.h>
#include <LoRa.h>

void sendLoRaMessage(const String &recipient, const String &message);

#endif // LORA_HELPER_H

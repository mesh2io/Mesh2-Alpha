#ifndef LORA_USER_LIST_H
#define LORA_USER_LIST_H

#include <Arduino.h>

// Declare the functions
void saveLoRaUserList(const String &userList);
String getLoRaUserList();

#endif // LORA_USER_LIST_H

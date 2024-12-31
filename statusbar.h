#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <LovyanGFX.hpp>
#include "defaults.h"
#include "battery.h"

void updateStatusBar();
void setNewMessageIndicator(bool hasNewMessage);
void clearNewMessageFlag(); // Add this declaration

#endif // STATUSBAR_H

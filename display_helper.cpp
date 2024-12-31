#include "display_helper.h"
#include "TDeckConfig.h"
#include <LovyanGFX.hpp>


extern LGFX_S3TDeck display;

void displayMessage(const char *message) {
    display.fillRect(0, 30, 320, 200, TFT_BLACK); // Clear the display area
    display.setCursor(10, 100);
    display.setTextColor(TFT_WHITE);
    display.println(message);
    display.display();
    delay(2000); // Pause to show the message
    return;
}

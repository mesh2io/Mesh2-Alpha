#include "read_messages.h"
#include "display_helper.h"
#include "storage.h"
#include "TDeckConfig.h"
#include <LovyanGFX.hpp>

extern LGFX_S3TDeck display;

void readMessages() {
    String allMessages = readStoredMessages();
    if (allMessages.isEmpty()) {
        displayMessage("No messages found");
        return;
    }

    display.fillRect(0, 30, 320, 200, TFT_BLACK);
    display.setCursor(10, 50);
    display.setTextColor(TFT_WHITE);
    display.println("Messages:");

    int line = 0;
    int startIdx = 0;
    while (startIdx < allMessages.length()) {
        int endIdx = allMessages.indexOf('\n', startIdx);
        if (endIdx == -1) endIdx = allMessages.length();

        String message = allMessages.substring(startIdx, endIdx);
        display.setCursor(10, 70 + (line * 20));
        display.println(message);
        line++;

        startIdx = endIdx + 1;
    }
    display.display();
    clearNewMessageFlag();
}

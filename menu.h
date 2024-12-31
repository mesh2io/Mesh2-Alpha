#ifndef MENU_H
#define MENU_H

#include <LovyanGFX.hpp>
#include "TDeckConfig.h" // Include this to define LGFX_S3TDeck
#include "keyboard_settings.h"
#include "storage.h"
#include "lora_settings.h"
#include "statusbar.h"

// Declare the global display object
extern LGFX_S3TDeck display;

// Function declarations
void waitForTrackballRelease(); // Add this line
void drawMainMenu();
void handleMenuInput();
void navigateMenu(bool up);
void selectMenuItem();
void drawConfigureMenu();
void navigateConfigureMenu(bool up);
void selectConfigureMenuItem();

#endif // MENU_H

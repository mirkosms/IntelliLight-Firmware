#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

#include <FastLED.h>
#include "config.h"

// Globalna deklaracja tablicy LED
extern CRGB leds[NUM_LEDS];

// Funkcje do obsługi LED
void initLEDs();
void setAllLEDs(int r, int g, int b);
void clearLEDs();
void showLEDs();

#endif

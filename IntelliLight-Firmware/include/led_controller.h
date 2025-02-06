#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

#include <FastLED.h>
#include "config.h"

// Globalna deklaracja tablicy LED
extern CRGB leds[NUM_LEDS];

// Inicjalizacja LED
void initLEDs();

// Ustawienia kolorów
void setAllLEDs(int r, int g, int b);              // Ustawienie RGB
void setWhiteTemperature(int cool, int warm);      // Ustawienie temperatury białego światła
void setCustomColor(int r, int g, int b, int w);   // Mieszanka RGB i bieli

// Efekty świetlne
void fadeToColor(int r, int g, int b, int duration);
void calibrateLEDs();
// Wyświetlanie i czyszczenie
void clearLEDs();
void showLEDs();

#endif

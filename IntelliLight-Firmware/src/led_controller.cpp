#include "led_controller.h"

// Definicja globalnej tablicy LED
CRGB leds[NUM_LEDS];

void initLEDs() {
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
    FastLED.clear();
    FastLED.show();
}

void setAllLEDs(int r, int g, int b) {
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB(r, g, b);
    }
}

void clearLEDs() {
    FastLED.clear();
}

void showLEDs() {
    FastLED.show();
}

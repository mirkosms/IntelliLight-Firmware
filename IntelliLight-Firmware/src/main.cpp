#include <FastLED.h>

#define LED_PIN 5       // Pin ESP32 podłączony do Data In taśmy LED
#define NUM_LEDS 30     // Liczba diod na taśmie

CRGB leds[NUM_LEDS];

void setup() {
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
    FastLED.clear();
    FastLED.show();
}

void loop() {
    // Wszystkie diody świecą na biało (maksymalna jasność 255)
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB(255, 255, 255);
    }
    FastLED.show();
    delay(1000);  // Odświeżanie co 1s
}

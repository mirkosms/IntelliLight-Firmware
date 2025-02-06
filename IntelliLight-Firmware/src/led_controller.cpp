#include "led_controller.h"

CRGB leds[NUM_LEDS];

void initLEDs() {
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(180);
    FastLED.clear(true);
    FastLED.show();
    delay(100);
}

// Ustawienie koloru RGB
void setAllLEDs(int r, int g, int b) {
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB(r, g, b);
    }
    FastLED.show();
}

// Ustawienie temperatury białego światła
void setWhiteTemperature(int cool, int warm) {
    for (int i = 0; i < NUM_LEDS; i++) {
        int red = 0, green = 0, blue = 0;

        if (cool > 0 && warm == 0) {
            blue = cool * 0.4;
            green = cool * 0.4;
            red = cool * 0.15;
        } else if (warm > 0 && cool == 0) {
            red = warm * 0.6;
            green = warm * 0.3;
            blue = warm * 0.1;
        } else {
            red = (warm * 0.5) + (cool * 0.3);
            green = (cool + warm) / 3;
            blue = cool * 0.4;
        }

        red = constrain(red, 0, 255);
        green = constrain(green, 0, 255);
        blue = constrain(blue, 0, 255);

        leds[i] = CRGB(red, green, blue);
    }
    FastLED.show();
}

// REGULACJA JASNOŚCI
void setBrightness(int brightness) {
    brightness = constrain(brightness, 0, 255);
    FastLED.setBrightness(brightness);
    FastLED.show();
}

// EFEKT TĘCZY
void rainbowEffect(int wait) {
    for (int hue = 0; hue < 255; hue++) {
        for (int i = 0; i < NUM_LEDS; i++) {
            leds[i] = CHSV((hue + i * 10) % 255, 255, 255);
        }
        FastLED.show();
        delay(wait);
    }
}

// EFEKT MIGOTANIA
void twinkleEffect(CRGB color, int chance, int speed) {
    for (int i = 0; i < NUM_LEDS; i++) {
        if (random(100) < chance) {
            leds[i] = color;
        } else {
            leds[i] = CRGB::Black;
        }
    }
    FastLED.show();
    delay(speed);
}

// STEROWANIE STREFAMI LED
void setZoneColor(int start, int end, CRGB color) {
    start = constrain(start, 0, NUM_LEDS - 1);
    end = constrain(end, start, NUM_LEDS - 1);
    for (int i = start; i <= end; i++) {
        leds[i] = color;
    }
    FastLED.show();
}

// EFEKT PULSACJI
void pulsingEffect(CRGB color, int speed) {
    for (int brightness = 0; brightness <= 255; brightness += 5) {
        FastLED.setBrightness(brightness);
        fill_solid(leds, NUM_LEDS, color);
        FastLED.show();
        delay(speed);
    }
    for (int brightness = 255; brightness >= 0; brightness -= 5) {
        FastLED.setBrightness(brightness);
        fill_solid(leds, NUM_LEDS, color);
        FastLED.show();
        delay(speed);
    }
}

// TRYB NOCNY
void nightMode() {
    for (int brightness = 255; brightness >= 50; brightness -= 5) {
        FastLED.setBrightness(brightness);
        setAllLEDs(255, 244, 229);  // Ciepłe światło
        FastLED.show();
        delay(50);  // Szybkość przejścia
    }
}


// CZYSZCZENIE I POKAZANIE LED
void clearLEDs() {
    FastLED.clear();
}

void showLEDs() {
    FastLED.show();
}

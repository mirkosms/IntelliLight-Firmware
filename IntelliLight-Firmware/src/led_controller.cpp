#include "led_controller.h"

CRGB leds[NUM_LEDS];

void initLEDs() {
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);  // Ustawienie poprawnego układu GRB
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
}

// Ustawienie temperatury białego światła
void setWhiteTemperature(int cool, int warm) {
    for (int i = 0; i < NUM_LEDS; i++) {
        int red = 0, green = 0, blue = 0;

        if (cool > 0 && warm == 0) {
            // Zimne światło:
            blue = cool * 0.4;
            green = cool * 0.4;
            red = cool * 0.15;
        } else if (warm > 0 && cool == 0) {
            // Ciepłe światło:
            red = warm * 0.6;
            green = warm * 0.3;
            blue = warm * 0.1;
        } else {
            // Neutralne światło:
            red = (warm * 0.5) + (cool * 0.3);
            green = (cool + warm) / 3;
            blue = cool * 0.4;
        }

        // Ograniczenie wartości do bezpiecznego zakresu
        red = constrain(red, 0, 255);
        green = constrain(green, 0, 255);
        blue = constrain(blue, 0, 255);

        leds[i] = CRGB(red, green, blue);
    }
}


// Mieszanka RGB i bieli
void setCustomColor(int r, int g, int b, int w) {
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB(r + w, g + w, b + w);  // Dodanie bieli do RGB
    }
}

// Efekt płynnego przejścia do koloru
void fadeToColor(int r, int g, int b, int duration) {
    int steps = 50;
    for (int step = 0; step <= steps; step++) {
        int red = (r * step) / steps;
        int green = (g * step) / steps;
        int blue = (b * step) / steps;
        setAllLEDs(red, green, blue);
        showLEDs();
        delay(duration / steps);
    }
}

void calibrateLEDs() {
    setAllLEDs(255, 0, 0);  // Czerwony
    showLEDs();
    delay(2000);

    setAllLEDs(0, 255, 0);  // Zielony
    showLEDs();
    delay(2000);

    setAllLEDs(0, 0, 255);  // Niebieski
    showLEDs();
    delay(2000);

    setAllLEDs(255, 255, 255);  // Biały
    showLEDs();
    delay(2000);
}

void clearLEDs() {
    FastLED.clear();
}

void showLEDs() {
    FastLED.show();
}

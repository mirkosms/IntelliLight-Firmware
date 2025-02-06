#include "led_controller.h"

CRGB leds[NUM_LEDS];

void initLEDs() {
    FastLED.addLeds<WS2812B, LED_PIN, RGB>(leds, NUM_LEDS);  // Zmieniamy z GRB na RGB
    FastLED.setBrightness(180);

    // Dodajemy korekcję kolorów dla taśmy WS2812B
    // FastLED.setCorrection(TypicalLEDStrip);  // Korekcja kolorów specyficzna dla WS2812B

    // Pełne czyszczenie diod przy starcie, aby usunąć efekt GRB
    FastLED.clear(true);  // Wymuszenie czyszczenia wszystkich diod
    FastLED.show();       // Wysyłanie pustych danych, aby zresetować diody
    delay(100);           // Krótkie opóźnienie na zresetowanie diod
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
            // Zimne światło: dominacja niebieskiego, lekka domieszka zieleni
            blue = cool;
            green = cool / 6;  // Minimalna domieszka zieleni dla naturalności

        } else if (warm > 0 && cool == 0) {
            // Ciepłe światło: dominacja czerwonego, ZIELONY WYŁĄCZONY
            red = warm;
            green = warm / 20;  // Minimalny zielony dla żółtego efektu
            blue = 0;  // Brak niebieskiego

        } else {
            // Neutralne światło: równowaga czerwieni, zieleni i niebieskiego
            red = warm / 2 + cool / 3;
            green = (cool + warm) / 4;
            blue = cool / 2;
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

// Funkcja kalibracyjna – test kolorów podstawowych
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

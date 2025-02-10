#include "led_controller.h"

LEDController::LEDController() {
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(180);
    isRainbowActive = false;
    lastUpdate = 0;
    rainbowHue = 0;
}

void LEDController::init() {
    clear();
    show();
    delay(100);
}

void LEDController::clear() {
    FastLED.clear();
    show();
    isRainbowActive = false;  // Wyłączenie efektu rainbow przy czyszczeniu LED
}

void LEDController::show() {
    FastLED.show();
}

void LEDController::setAll(int r, int g, int b) {
    fill_solid(leds, NUM_LEDS, CRGB(r, g, b));
    show();
}

void LEDController::rainbowEffect() {
    isRainbowActive = true;   // Włącz efekt rainbow
}

void LEDController::updateRainbow() {
    if (isRainbowActive && millis() - lastUpdate > 50) {  // Aktualizacja co 50 ms
        for (int i = 0; i < NUM_LEDS; i++) {
            leds[i] = CHSV((rainbowHue + i * 10) % 255, 255, 255);
        }
        show();
        rainbowHue++;
        lastUpdate = millis();
    }
}

void LEDController::stopRainbow() {
    isRainbowActive = false;
    clear();  // Wyłącz LED-y po zatrzymaniu efektu
}

// Pozostałe efekty LED
void LEDController::setWhiteTemperature(int cool, int warm) {
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

        leds[i] = CRGB(constrain(red, 0, 255), constrain(green, 0, 255), constrain(blue, 0, 255));
    }
    show();
}

void LEDController::setBrightness(int brightness) {
    FastLED.setBrightness(constrain(brightness, 0, 255));
    show();
}

void LEDController::twinkleEffect(const CRGB& color, int chance, int speed) {
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = random(100) < chance ? color : CRGB::Black;
    }
    show();
    delay(speed);
}

void LEDController::setZoneColor(int start, int end, const CRGB& color) {
    for (int i = constrain(start, 0, NUM_LEDS - 1); i <= constrain(end, start, NUM_LEDS - 1); i++) {
        leds[i] = color;
    }
    show();
}

void LEDController::pulsingEffect(const CRGB& color, int speed) {
    for (int brightness = 0; brightness <= 255; brightness += 5) {
        FastLED.setBrightness(brightness);
        fill_solid(leds, NUM_LEDS, color);
        show();
        delay(speed);
    }
    for (int brightness = 255; brightness >= 0; brightness -= 5) {
        FastLED.setBrightness(brightness);
        fill_solid(leds, NUM_LEDS, color);
        show();
        delay(speed);
    }
}

void LEDController::nightMode() {
    for (int brightness = 255; brightness >= 50; brightness -= 5) {
        FastLED.setBrightness(brightness);
        setAll(255, 244, 229);  // Ciepłe światło
        delay(50);
    }
}

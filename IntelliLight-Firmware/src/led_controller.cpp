#include "led_controller.h"

LEDController::LEDController() {
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(180);
    currentBrightness = 180; // Domyślna jasność
    isStaticActive = false;
    isRainbowActive = false;
    isPulsingActive = false;
    isNightModeActive = false;
    isTwinkleActive = false;
    isWhiteTempActive = false;
    lastUpdate = 0;
    rainbowHue = 0;
    pulsingBrightness = 0;
    pulsingDirection = 5;
}

void LEDController::init() {
    clear();
    show();
    delay(100);
}

void LEDController::clear() {
    FastLED.clear();
    show();
    isStaticActive = false;
    isRainbowActive = false;
    isPulsingActive = false;
    isNightModeActive = false;
    isTwinkleActive = false;
    isWhiteTempActive = false;
}

void LEDController::show() {
    FastLED.show();
}

void LEDController::setAll(int r, int g, int b) {
    fill_solid(leds, NUM_LEDS, CRGB(r, g, b));
    show();
}

void LEDController::toggleStatic() {
    if (isStaticActive) {
        clear();
    } else {
        clear();
        setAll(0, 0, 255);
        isStaticActive = true;
    }
}

void LEDController::toggleRainbow() {
    if (isRainbowActive) {
        clear();
    } else {
        clear();
        isRainbowActive = true;
    }
}

void LEDController::togglePulsing() {
    if (isPulsingActive) {
        clear();
    } else {
        clear();
        isPulsingActive = true;
    }
}

void LEDController::toggleNightMode() {
    if (isNightModeActive) {
        clear();
    } else {
        clear();
        for (int i = 0; i < NUM_LEDS; i++) {
            leds[i] = CRGB(255, 223, 186);
        }
        FastLED.setBrightness(80);
        show();
        isNightModeActive = true;
    }
}

void LEDController::toggleTwinkle() {
    if (isTwinkleActive) {
        clear();
    } else {
        clear();
        isTwinkleActive = true;
    }
}

void LEDController::toggleWhiteTemperature(const String& mode) {
    if (isWhiteTempActive && whiteTempMode == mode) {
        clear();
    } else {
        clear();
        if (mode == "neutral") {
            setWhiteTemperature(127, 127);
        } else if (mode == "cool") {
            setWhiteTemperature(255, 50);
        } else if (mode == "warm") {
            setWhiteTemperature(50, 255);
        }
        isWhiteTempActive = true;
        whiteTempMode = mode;
    }
}

void LEDController::updateEffects() {
    if (isRainbowActive && millis() - lastUpdate > 50) {
        for (int i = 0; i < NUM_LEDS; i++) {
            leds[i] = CHSV((rainbowHue + i * 10) % 255, 255, currentBrightness);
        }
        show();
        rainbowHue++;
        lastUpdate = millis();
    }

    if (isPulsingActive && millis() - lastUpdate > 50) {
        FastLED.setBrightness(pulsingBrightness);
        fill_solid(leds, NUM_LEDS, CRGB::Blue);
        show();
        pulsingBrightness += pulsingDirection;
        if (pulsingBrightness >= 255 || pulsingBrightness <= 0) {
            pulsingDirection = -pulsingDirection;
        }
        lastUpdate = millis();
    }

    if (isTwinkleActive && millis() - lastUpdate > 150) {
        for (int i = 0; i < NUM_LEDS; i++) {
            leds[i] = random(10) > 7 ? CRGB::White : CRGB::Black;
        }
        show();
        lastUpdate = millis();
    }
}

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

void LEDController::setAutoBrightness(float lux) {
    int brightness = map(lux, 0, 500, 255, 50);  // Skalowanie jasności: od ciemności (255) do jasnego pokoju (50)
    brightness = constrain(brightness, 50, 255); // Ograniczenie wartości
    setBrightness(brightness);
}

void LEDController::setBrightness(int brightness) {
    currentBrightness = constrain(brightness, 0, 255);
    FastLED.setBrightness(currentBrightness);
    show();
}

int LEDController::getBrightness() {
    return currentBrightness;
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
        setAll(255, 244, 229);
        delay(50);
    }
}

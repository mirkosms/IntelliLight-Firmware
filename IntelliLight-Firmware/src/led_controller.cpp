#include "led_controller.h"

LEDController::LEDController() : manualOverride(false), lastActiveEffect("none") {
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
    currentBrightness = 180; // Domyślna jasność
    FastLED.setBrightness(currentBrightness);
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
    // Resetujemy flagi wszystkich efektów
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
        setManualOverride(false);
    } else {
        clear();
        setAll(0, 0, 255);
        isStaticActive = true;
        setLastActiveEffect("static");
        setManualOverride(true);
    }
}

void LEDController::toggleRainbow() {
    if (isRainbowActive) {
        clear();
        setManualOverride(false);
    } else {
        clear();
        isRainbowActive = true;
        setLastActiveEffect("rainbow");
        setManualOverride(true);
    }
}

void LEDController::togglePulsing() {
    if (isPulsingActive) {
        clear();
        setManualOverride(false);
    } else {
        clear();
        isPulsingActive = true;
        setLastActiveEffect("pulsing");
        setManualOverride(true);
    }
}

void LEDController::toggleNightMode() {
    if (isNightModeActive) {
        clear();
        setManualOverride(false);
    } else {
        clear();
        for (int i = 0; i < NUM_LEDS; i++) {
            leds[i] = CRGB(255, 223, 186);
        }
        FastLED.setBrightness(80);
        show();
        isNightModeActive = true;
        setLastActiveEffect("night");
        setManualOverride(true);
    }
}

void LEDController::toggleTwinkle() {
    if (isTwinkleActive) {
        clear();
        setManualOverride(false);
    } else {
        clear();
        isTwinkleActive = true;
        setLastActiveEffect("twinkle");
        setManualOverride(true);
    }
}

void LEDController::toggleWhiteTemperature(const String& mode) {
    if (isWhiteTempActive && whiteTempMode == mode) {
        clear();
        setManualOverride(false);
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
        setLastActiveEffect("static");
    }
}

// Prywatne funkcje pomocnicze aktualizujące efekty

void LEDController::updateRainbowEffect() {
    if (millis() - lastUpdate > 50) {
        for (int i = 0; i < NUM_LEDS; i++) {
            leds[i] = CHSV((rainbowHue + i * 10) % 255, 255, currentBrightness);
        }
        show();
        rainbowHue++;
        lastUpdate = millis();
    }
}

void LEDController::updatePulsingEffect() {
    if (millis() - lastUpdate > 50) {
        FastLED.setBrightness(pulsingBrightness);
        fill_solid(leds, NUM_LEDS, CRGB::Blue);
        show();
        pulsingBrightness += pulsingDirection;
        if (pulsingBrightness >= 255 || pulsingBrightness <= 0) {
            pulsingDirection = -pulsingDirection;
        }
        lastUpdate = millis();
    }
}

void LEDController::updateTwinkleEffect() {
    if (millis() - lastUpdate > 150) {
        for (int i = 0; i < NUM_LEDS; i++) {
            leds[i] = (random(10) > 7) ? CRGB::White : CRGB::Black;
        }
        show();
        lastUpdate = millis();
    }
}

void LEDController::updateEffects() {
    // Aktualizacja ciągłych efektów odbywa się osobno dla każdego trybu
    if (isRainbowActive) {
        updateRainbowEffect();
    }
    if (isPulsingActive) {
        updatePulsingEffect();
    }
    if (isTwinkleActive) {
        updateTwinkleEffect();
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
    int brightness = map(lux, 0, 500, 255, 50);  // Od ciemności (255) do jasności (50)
    brightness = constrain(brightness, 50, 255);
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
        leds[i] = (random(100) < chance) ? color : CRGB::Black;
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

bool LEDController::isAnyEffectActive() {
    return isStaticActive || isRainbowActive || isPulsingActive ||
           isNightModeActive || isTwinkleActive || isWhiteTempActive;
}

void LEDController::setManualOverride(bool state) {
    manualOverride = state;
}

bool LEDController::isManualOverride() {
    return manualOverride;
}

void LEDController::setLastActiveEffect(const String& effect) {
    lastActiveEffect = effect;
}

String LEDController::getLastActiveEffect() {
    return lastActiveEffect;
}

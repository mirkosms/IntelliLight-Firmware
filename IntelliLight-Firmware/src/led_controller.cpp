#include "led_controller.h"

LEDController::LEDController() : manualOverride(false), lastActiveEffect("none"), leds_on(false) {
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
    currentBrightness = 180; // Domyślna jasność
    FastLED.setBrightness(currentBrightness);
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

// Wersja z parametrem preserve_effect
void LEDController::clear(bool preserve_effect) {
    FastLED.clear();
    show();
    isRainbowActive = false;
    isPulsingActive = false;
    isNightModeActive = false;
    isTwinkleActive = false;
    isWhiteTempActive = false;
    leds_on = false;
    if (!preserve_effect) {
        lastActiveEffect = "none";
    }
}

void LEDController::clear() {
    clear(false);
}

void LEDController::show() {
    FastLED.show();
}

void LEDController::setAll(int r, int g, int b) {
    fill_solid(leds, NUM_LEDS, CRGB(r, g, b));
    leds_on = true;
    show();
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
    leds_on = true;
    show();
}

void LEDController::setBrightness(int brightness) {
    currentBrightness = constrain(brightness, 0, 255);
    FastLED.setBrightness(currentBrightness);
    show();
}

int LEDController::getBrightness() {
    return currentBrightness;
}

void LEDController::setAutoBrightness(float lux) {
    int brightness = map(lux, 0, 500, 255, 50);  // Od ciemności (255) do jasności (50)
    brightness = constrain(brightness, 50, 255);
    setBrightness(brightness);
}

void LEDController::setZoneColor(int start, int end, const CRGB& color) {
    for (int i = constrain(start, 0, NUM_LEDS - 1); i <= constrain(end, start, NUM_LEDS - 1); i++) {
        leds[i] = color;
    }
    leds_on = true;
    show();
}

void LEDController::twinkleEffect(const CRGB& color, int chance, int speed) {
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = (random(100) < chance) ? color : CRGB::Black;
    }
    leds_on = true;
    show();
    delay(speed);
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
    leds_on = true;
}

void LEDController::nightMode() {
    // Ustawienie night mode jako trybu warm white, identycznego jak w trybie white z parametrem "warm"
    setWhiteTemperature(50, 255);
    FastLED.setBrightness(80);
    leds_on = true;
    show();
    isNightModeActive = true;
}

// Jednolita funkcja toggle_effect
void LEDController::toggle_effect(const String &effect, const String &param, bool forceActivate) {
    if (!forceActivate && lastActiveEffect == effect) {
        if (effect == "white") {
            // Dla trybu white, jeśli już taki tryb jest aktywny i parametr się zgadza, wyłącz efekt.
            if (get_white_temp_mode() == param) {
                clear();
                setManualOverride(false);
                lastActiveEffect = "none";
                return;
            }
            // Jeśli parametr inny, przełącz na nowy tryb.
        } else {
            clear();
            setManualOverride(false);
            lastActiveEffect = "none";
            return;
        }
    }
    clear();
    if (effect == "static") {
        setAll(0, 0, 255);
    } else if (effect == "rainbow") {
        isRainbowActive = true;
    } else if (effect == "pulsing") {
        isPulsingActive = true;
    } else if (effect == "night") {
        nightMode();
    } else if (effect == "twinkle") {
        isTwinkleActive = true;
    } else if (effect == "white") {
        if (param == "neutral") {
            setWhiteTemperature(127, 127);
        } else if (param == "cool") {
            setWhiteTemperature(255, 50);
        } else if (param == "warm") {
            setWhiteTemperature(50, 255);
        }
        isWhiteTempActive = true;
        whiteTempMode = param;
    }
    lastActiveEffect = effect;
    setManualOverride(true);
}

bool LEDController::isAnyEffectActive() {
    return (lastActiveEffect != "none");
}

bool LEDController::is_led_on() {
    return leds_on;
}

void LEDController::setManualOverride(bool state) {
    manualOverride = state;
}

bool LEDController::isManualOverride() {
    return manualOverride;
}

void LEDController::setLastActiveEffect(const String &effect) {
    lastActiveEffect = effect;
}

String LEDController::getLastActiveEffect() {
    return lastActiveEffect;
}

// Nowa metoda – zwraca bieżący podtryb dla trybu "white"
String LEDController::get_white_temp_mode() {
    return whiteTempMode;
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
            leds[i] = (random(10) > 7) ? CRGB::White : CRGB::Black;
        }
        show();
        lastUpdate = millis();
    }
}

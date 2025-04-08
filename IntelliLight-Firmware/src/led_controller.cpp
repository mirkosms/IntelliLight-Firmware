#include "led_controller.h"

LEDController::LEDController() 
    : manualOverride(false), lastActiveEffect("none"), leds_on(false),
      isRainbowActive(false), isPulsingActive(false), isNightModeActive(false),
      isTwinkleActive(false), isWhiteTempActive(false),
      lastUpdate(0), rainbowHue(0), pulsingBrightness(0), pulsingDirection(5),
      currentBrightness(180)
{
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(currentBrightness);
}

void LEDController::init() {
    clear();
    show();
    delay(100);
}

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
    Serial.print("LED: Ustawiam jasność na: ");
    Serial.println(brightness);

    currentBrightness = constrain(brightness, 0, 255);
    FastLED.setBrightness(currentBrightness);
    show();
}

int LEDController::getBrightness() {
    return currentBrightness;
}

void LEDController::setAutoBrightness(float lux) {
    int brightness = map(lux, 0, 500, 255, 50);
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
    setWhiteTemperature(50, 255);
    FastLED.setBrightness(80);
    leds_on = true;
    show();
    isNightModeActive = true;
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

String LEDController::get_white_temp_mode() {
    return whiteTempMode;
}

void LEDController::set_white_temp_mode(const String &mode) {
    whiteTempMode = mode;
}

void LEDController::setCustomColor(int r, int g, int b) {
    customColor = CRGB(r, g, b);
    setAll(r, g, b);
}

CRGB LEDController::getCustomColor() {
    return customColor;
}

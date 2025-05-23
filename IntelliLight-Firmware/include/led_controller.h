#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

#include <FastLED.h>
#include "config.h"

class LEDController {
public:
    LEDController();
    void init();
    
    void clear(bool preserve_effect);
    void clear();
    
    void show();
    void setAll(int r, int g, int b);
    void setWhiteTemperature(int cool, int warm);
    void setBrightness(int brightness);
    void setAutoBrightness(float lux);
    void setZoneColor(int start, int end, const CRGB& color);
    void twinkleEffect(const CRGB& color, int chance, int speed);
    void pulsingEffect(const CRGB& color, int speed);
    void nightMode();
    void updateEffects();

    // Gettery/settery
    int getBrightness();
    bool isAnyEffectActive();
    bool is_led_on();
    void setManualOverride(bool state);
    bool isManualOverride();
    void setLastActiveEffect(const String &effect);
    String getLastActiveEffect();
    String get_white_temp_mode();
    void set_white_temp_mode(const String &mode);
    void setCustomColor(int r, int g, int b);
    CRGB getCustomColor();

    // Flagi dla efektów ciągłych
    bool isRainbowActive;
    bool isPulsingActive;
    bool isNightModeActive;
    bool isTwinkleActive;
    bool isWhiteTempActive;

private:
    CRGB leds[NUM_LEDS];
    String lastActiveEffect;
    String whiteTempMode;
    CRGB customColor;

    unsigned long lastUpdate;
    uint8_t rainbowHue;
    int pulsingBrightness;
    int pulsingDirection;
    int currentBrightness;
    bool manualOverride;
    bool leds_on;
};

#endif // LED_CONTROLLER_H

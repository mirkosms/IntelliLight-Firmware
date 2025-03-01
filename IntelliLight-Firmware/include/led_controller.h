#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

#include <FastLED.h>
#include "config.h"

class LEDController {
public:
    LEDController();
    void init();
    void clear();
    void show();
    void setAll(int r, int g, int b);
    void setWhiteTemperature(int cool, int warm);
    void setBrightness(int brightness);
    void setAutoBrightness(float lux);  // Automatyczna regulacja jasności
    void setZoneColor(int start, int end, const CRGB& color);
    void twinkleEffect(const CRGB& color, int chance, int speed);
    void pulsingEffect(const CRGB& color, int speed);
    void nightMode();

    // Funkcje przełączające tryby efektów LED
    void toggleStatic();
    void toggleRainbow();
    void togglePulsing();
    void toggleNightMode();
    void toggleTwinkle();
    void toggleWhiteTemperature(const String& mode);
    
    // Aktualizacja efektów ciągłych (np. rainbow, pulsing, twinkle)
    void updateEffects();

    int getBrightness();
    bool isAnyEffectActive();
    void setManualOverride(bool state);
    bool isManualOverride();
    void setLastActiveEffect(const String& effect);
    String getLastActiveEffect();

private:
    CRGB leds[NUM_LEDS];
    bool isStaticActive;
    bool isRainbowActive;
    bool isPulsingActive;
    bool isNightModeActive;
    bool isTwinkleActive;
    bool isWhiteTempActive;
    String whiteTempMode;
    unsigned long lastUpdate;
    uint8_t rainbowHue;
    int pulsingBrightness;
    int pulsingDirection;
    int currentBrightness;  // Aktualna jasność LED
    bool manualOverride;
    String lastActiveEffect;

    // Prywatne funkcje pomocnicze aktualizujące poszczególne efekty
    void updateRainbowEffect();
    void updatePulsingEffect();
    void updateTwinkleEffect();
};

#endif // LED_CONTROLLER_H

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
    void setZoneColor(int start, int end, const CRGB& color);
    void twinkleEffect(const CRGB& color, int chance, int speed);
    void pulsingEffect(const CRGB& color, int speed);
    void nightMode();

    void toggleStatic();
    void toggleRainbow();
    void togglePulsing();
    void updateEffects();

private:
    CRGB leds[NUM_LEDS];
    bool isStaticActive;
    bool isRainbowActive;
    bool isPulsingActive;
    unsigned long lastUpdate;
    uint8_t rainbowHue;
    int pulsingBrightness;
    int pulsingDirection;
};

#endif // LED_CONTROLLER_H

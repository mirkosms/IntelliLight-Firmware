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
    void rainbowEffect();       // Nowa metoda do uruchomienia efektu
    void updateRainbow();       // Aktualizacja efektu rainbow w pętli
    void stopRainbow();         // Zatrzymanie efektu rainbow
    void twinkleEffect(const CRGB& color, int chance, int speed);
    void setZoneColor(int start, int end, const CRGB& color);
    void pulsingEffect(const CRGB& color, int speed);
    void nightMode();

private:
    CRGB leds[NUM_LEDS];
    bool isRainbowActive;       // Flaga wskazująca, czy efekt rainbow jest aktywny
    unsigned long lastUpdate;   // Czas ostatniej aktualizacji efektu
    uint8_t rainbowHue;         // Bieżący kolor hue dla efektu rainbow
};

#endif // LED_CONTROLLER_H

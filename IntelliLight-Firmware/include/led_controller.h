#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

#include <FastLED.h>
#include "config.h"

class LEDController {
public:
    LEDController();
    void init();
    // Przeciążona funkcja clear – gdy preserve_effect jest true, nie zerujemy lastActiveEffect
    void clear(bool preserve_effect);
    // Domyślne wywołanie clear() (bez zachowania efektu)
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

    // Jednolita funkcja przełączająca efekty LED.
    // effect: "static", "rainbow", "pulsing", "night", "twinkle", "white"
    // dla trybu "white" parametr dodatkowy określa tryb białej temperatury ("neutral", "cool", "warm")
    // forceActivate: gdy true, efekt jest aktywowany niezależnie od stanu przechowywanego w lastActiveEffect
    void toggle_effect(const String &effect, const String &param = "", bool forceActivate = false);

    int getBrightness();
    bool isAnyEffectActive();
    bool is_led_on();  // Sprawdza, czy diody są włączone
    void setManualOverride(bool state);
    bool isManualOverride();
    void setLastActiveEffect(const String &effect);
    String getLastActiveEffect();

    // Nowa metoda – umożliwia pobranie bieżącego podtrybu dla trybu "white"
    String get_white_temp_mode();

    // Aktualizacja ciągłych efektów LED.
    void updateEffects();

private:
    CRGB leds[NUM_LEDS];
    String lastActiveEffect;  // Aktualnie aktywny efekt ("none" jeśli brak)

    // Flag dla efektów ciągłych
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
    bool leds_on;  // Informacja, czy diody są włączone
};

#endif // LED_CONTROLLER_H

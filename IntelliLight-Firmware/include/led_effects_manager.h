#ifndef LED_EFFECTS_MANAGER_H
#define LED_EFFECTS_MANAGER_H

#include "led_controller.h"

class LEDEffectsManager {
public:
    LEDEffectsManager(LEDController &led_controller);

    // Przełączanie efektu LED.
    // effect: "static", "rainbow", "pulsing", "night", "twinkle", "white"
    // param: np. "neutral", "cool", "warm" dla trybu white
    // forceActivate: gdy true, efekt jest aktywowany nawet jeśli jest już ustawiony
    void toggle_effect(const String &effect, const String &param = "", bool forceActivate = false);

    // (Nie przenosimy updateEffects – efekty ciągłe obsługuje LEDController::updateEffects().)
    void update_effects(); // placeholder

private:
    LEDController &led_controller;
};

#endif // LED_EFFECTS_MANAGER_H

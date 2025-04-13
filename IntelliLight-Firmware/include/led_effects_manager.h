#ifndef LED_EFFECTS_MANAGER_H
#define LED_EFFECTS_MANAGER_H

#include "led_controller.h"

class LEDEffectsManager {
public:
    LEDEffectsManager(LEDController &led_controller);
    
    // Przełączanie efektu LED.
    // effect: "static", "rainbow", "pulsing", "night", "twinkle", "white", "custom"
    // param: dla trybu white: "neutral", "cool", "warm"
    // forceActivate: gdy true, wymusza ponowne ustawienie efektu
    void toggle_effect(const String &effect, const String &param = "", bool forceActivate = false);
    
    void update_effects();
    
private:
    LEDController &led_controller;
};

#endif // LED_EFFECTS_MANAGER_H

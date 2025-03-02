#include "led_effects_manager.h"

LEDEffectsManager::LEDEffectsManager(LEDController &led_controller)
    : led_controller(led_controller)
{}

void LEDEffectsManager::toggle_effect(const String &effect, const String &param, bool forceActivate) {
    if (!forceActivate && led_controller.getLastActiveEffect() == effect) {
        if (effect == "white") {
            if (led_controller.get_white_temp_mode() == param) {
                led_controller.clear();
                led_controller.setManualOverride(false);
                led_controller.setLastActiveEffect("none");
                return;
            }
        } else {
            led_controller.clear();
            led_controller.setManualOverride(false);
            led_controller.setLastActiveEffect("none");
            return;
        }
    }
    led_controller.clear();
    if (effect == "static") {
        led_controller.setAll(0, 0, 255);
        led_controller.setLastActiveEffect("static");
    } else if (effect == "rainbow") {
        led_controller.isRainbowActive = true;
        led_controller.setLastActiveEffect("rainbow");
    } else if (effect == "pulsing") {
        led_controller.isPulsingActive = true;
        led_controller.setLastActiveEffect("pulsing");
    } else if (effect == "night") {
        led_controller.nightMode();
        led_controller.setLastActiveEffect("night");
    } else if (effect == "twinkle") {
        led_controller.isTwinkleActive = true;
        led_controller.setLastActiveEffect("twinkle");
    } else if (effect == "white") {
        if (param == "neutral") {
            led_controller.setWhiteTemperature(127, 127);
        } else if (param == "cool") {
            led_controller.setWhiteTemperature(255, 50);
        } else if (param == "warm") {
            led_controller.setWhiteTemperature(50, 255);
        }
        led_controller.isWhiteTempActive = true;
        led_controller.set_white_temp_mode(param);
        led_controller.setLastActiveEffect("white");
    }
    led_controller.setManualOverride(true);
}

void LEDEffectsManager::update_effects() {
    // Aktualizacja efektów ciągłych obsługiwana przez LEDController::updateEffects()
}

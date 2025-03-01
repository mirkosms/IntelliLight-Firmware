#include "environment_controller.h"
#include <Arduino.h>

EnvironmentController::EnvironmentController(Sensors &s, LEDController &l, PomodoroTimer &p,
                                               bool &autoBright, bool &motion, unsigned long &timeout)
    : sensors(s), ledController(l), pomodoro(p), autoBrightnessEnabled(autoBright),
      motionEnabled(motion), motionTimeout(timeout), lastMotionTime(0) {}

void EnvironmentController::update() {
    updateAutoBrightness();
    updateMotionSensor();
}

void EnvironmentController::updateAutoBrightness() {
    if (autoBrightnessEnabled) {
        String activeEffect = ledController.getLastActiveEffect();
        // Jeśli aktywny jest tryb night, pulsing lub rainbow, auto brightness nie działa
        if (activeEffect == "night" || activeEffect == "pulsing" || activeEffect == "rainbow") return;
        float lux = sensors.readLightLevel();
        ledController.setAutoBrightness(lux);
    }
}

void EnvironmentController::updateMotionSensor() {
    if (motionEnabled && !pomodoro.isRunning()) {
        bool motionDetected = sensors.readMotion();
        if (motionDetected) {
            // Wprowadzamy debounce: przywracamy efekt tylko, gdy minęły co najmniej 2000 ms od ostatniego przywrócenia
            if (millis() - lastMotionTime > 2000) {
                lastMotionTime = millis();
                if (!ledController.is_led_on()) {
                    if (ledController.getLastActiveEffect() != "none") {
                        String lastEffect = ledController.getLastActiveEffect();
                        if (lastEffect == "white") {
                            ledController.toggle_effect("white", ledController.get_white_temp_mode(), true);
                        } else {
                            ledController.toggle_effect(lastEffect, "", true);
                        }
                    } else {
                        ledController.setAll(0, 0, 255);
                    }
                }
                ledController.setManualOverride(false);
            }
        } else if (millis() - lastMotionTime > motionTimeout) {
            Serial.println("Brak ruchu przez określony czas. Wyłączam LED.");
            // Przy wyłączaniu zachowujemy ostatni tryb
            ledController.clear(true);
            ledController.setManualOverride(false);
        }
    }
}

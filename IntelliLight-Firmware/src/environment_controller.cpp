#include "environment_controller.h"
#include "led_effects_manager.h"
#include <Arduino.h>

extern LEDEffectsManager ledEffectsMgr;

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
        if (activeEffect == "night" || activeEffect == "pulsing" || activeEffect == "rainbow") {
            return;
        }
        float lux = sensors.readLightLevel();
        ledController.setAutoBrightness(lux);
    }
}

void EnvironmentController::updateMotionSensor() {
    if (motionEnabled && !pomodoro.isRunning()) {
        bool motionDetected = sensors.readMotion();
        if (motionDetected) {
            if (millis() - lastMotionTime > 2000) { // debounce 2 sekundy
                lastMotionTime = millis();
                if (!ledController.is_led_on()) {
                    String lastEffect = ledController.getLastActiveEffect();
                    if (lastEffect == "none") {
                        ledController.setAll(0, 0, 255);
                    } else if (lastEffect == "white") {
                        ledEffectsMgr.toggle_effect("white", ledController.get_white_temp_mode(), true);
                    } else {
                        ledEffectsMgr.toggle_effect(lastEffect, "", true);
                    }
                }
                ledController.setManualOverride(false);
            }
        } else if (millis() - lastMotionTime > motionTimeout) {
            Serial.println("Brak ruchu przez określony czas. Wyłączam LED.");
            ledController.clear(true);
            ledController.setManualOverride(false);
        }
    }
}

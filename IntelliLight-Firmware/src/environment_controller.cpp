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
        float lux = sensors.readLightLevel();
        ledController.setAutoBrightness(lux);
    }
}

void EnvironmentController::updateMotionSensor() {
    if (motionEnabled && !pomodoro.isRunning()) {
        bool motionDetected = sensors.readMotion();
        if (motionDetected) {
            lastMotionTime = millis();
            if (!ledController.isAnyEffectActive()) {
                if (ledController.getLastActiveEffect() == "none") {
                    ledController.setAll(0, 0, 255);
                } else {
                    String lastEffect = ledController.getLastActiveEffect();
                    if (lastEffect == "static") ledController.toggleStatic();
                    else if (lastEffect == "rainbow") ledController.toggleRainbow();
                    else if (lastEffect == "pulsing") ledController.togglePulsing();
                    else if (lastEffect == "night") ledController.toggleNightMode();
                    else if (lastEffect == "twinkle") ledController.toggleTwinkle();
                }
                ledController.setManualOverride(false);
            }
        } else if (millis() - lastMotionTime > motionTimeout) {
            Serial.println("No motion detected for specified time. Turning off LEDs.");
            ledController.clear();
            ledController.setManualOverride(false);
        }
    }
}

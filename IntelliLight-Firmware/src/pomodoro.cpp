#include "pomodoro.h"

PomodoroTimer::PomodoroTimer(LEDController& ledController, WebServer& webServer)
    : leds(ledController), server(webServer) {}

void PomodoroTimer::startFocusSession(int focusMinutes) {
    totalMinutes = focusMinutes;
    ledsPerMinute = 1;
    sessionColor = CRGB::Green;
    elapsedMinutes = 0;
    startMillis = millis();
    running = true;
    leds.setAll(sessionColor.r, sessionColor.g, sessionColor.b);
}

void PomodoroTimer::startBreakSession(int breakMinutes) {
    totalMinutes = breakMinutes;
    ledsPerMinute = 6;
    sessionColor = CRGB::Red;
    elapsedMinutes = 0;
    startMillis = millis();
    running = true;
    leds.setAll(sessionColor.r, sessionColor.g, sessionColor.b);
}

void PomodoroTimer::resetTimer() {
    leds.clear();
    leds.show();
    running = false;
}

void PomodoroTimer::update() {
    if (!running) return;

    unsigned long currentMillis = millis();
    if (currentMillis - startMillis >= 60000) { // Minuta minęła
        startMillis = currentMillis;
        elapsedMinutes++;

        for (int i = 0; i < ledsPerMinute; i++) {
            int ledIndex = (elapsedMinutes * ledsPerMinute) + i;
            if (ledIndex < NUM_LEDS) {
                leds.setZoneColor(ledIndex, ledIndex, CRGB::Black);
            }
        }

        if (elapsedMinutes >= totalMinutes) {
            running = false;
            sessionCompleteEffect();
        }
    }
}

void PomodoroTimer::sessionCompleteEffect() {
    leds.setAll(255, 255, 255);
    delay(1000);
    leds.clear();
    leds.show();
}

bool PomodoroTimer::isRunning() {
    return running;
}
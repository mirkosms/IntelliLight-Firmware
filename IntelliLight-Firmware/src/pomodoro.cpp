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

    Serial.print("POMODORO: Rozpoczęcie sesji skupienia (");
    Serial.print(focusMinutes);
    Serial.println(" min)");
}

void PomodoroTimer::startBreakSession(int breakMinutes) {
    totalMinutes = breakMinutes;
    ledsPerMinute = 6;
    sessionColor = CRGB::Red;
    elapsedMinutes = 0;
    startMillis = millis();
    running = true;
    leds.setAll(sessionColor.r, sessionColor.g, sessionColor.b);

    Serial.print("POMODORO: Rozpoczęcie sesji przerwy (");
    Serial.print(breakMinutes);
    Serial.println(" min)");
}

void PomodoroTimer::resetTimer() {
    leds.clear();
    leds.show();
    running = false;
    Serial.println("POMODORO: Zresetowano timer");
}

void PomodoroTimer::update() {
    if (!running) return;
    unsigned long currentMillis = millis();
    if (currentMillis - startMillis >= 60000) { // Minuta minęła
        // Gaszenie LED reprezentujące postęp trybu Pomodoro dla bieżącej sesji
        for (int i = 0; i < ledsPerMinute; i++) {
            int ledIndex = (elapsedMinutes * ledsPerMinute) + i;
            if (ledIndex < NUM_LEDS) {
                leds.setZoneColor(ledIndex, ledIndex, CRGB::Black);
            }
        }
        elapsedMinutes++;             // Po wykonaniu pętli zwiększamy licznik minut
        startMillis = currentMillis;  // Aktualizacja punktu startowego czasu
        if (elapsedMinutes >= totalMinutes) {
            running = false;
            sessionCompleteEffect();
        }
    }
}


void PomodoroTimer::sessionCompleteEffect() {
    Serial.println("POMODORO: Sesja zakończona!");
    leds.setAll(255, 255, 255);
    delay(1000);
    leds.clear();
    leds.show();
}

bool PomodoroTimer::isRunning() {
    return running;
}

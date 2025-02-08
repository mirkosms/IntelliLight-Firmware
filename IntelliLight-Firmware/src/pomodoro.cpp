#include "pomodoro.h"

PomodoroTimer::PomodoroTimer(LEDController& ledController) : leds(ledController) {}

void PomodoroTimer::startFocusSession(int focusMinutes) {
    countdown(focusMinutes, CRGB::Green, 1);  // 1 dioda gaśnie co minutę
    sessionCompleteEffect();
}

void PomodoroTimer::startBreakSession(int breakMinutes) {
    countdown(breakMinutes, CRGB::Red, 6);  // 6 diod gasną co minutę
    sessionCompleteEffect();
}

void PomodoroTimer::resetTimer() {
    leds.clear();
    leds.show();
}

void PomodoroTimer::countdown(int totalMinutes, CRGB color, int ledsPerMinute) {
    leds.setAll(color.r, color.g, color.b);  // Ustaw wszystkie diody na kolor sesji

    for (int minute = 0; minute < totalMinutes; minute++) {
        delay(60000);  // Odczekaj minutę
        for (int i = 0; i < ledsPerMinute; i++) {
            int ledIndex = (minute * ledsPerMinute) + i;
            if (ledIndex < NUM_LEDS) {
                leds.setZoneColor(ledIndex, ledIndex, CRGB::Black);  // Zgaś diodę
            }
        }
    }
}

void PomodoroTimer::sessionCompleteEffect() {
    leds.setAll(255, 255, 255);  // Zapal wszystkie diody na biało na 1 sekundę
    delay(1000);
    leds.clear();
    leds.show();
}

#include "led_controller.h"
#include "pomodoro.h"

LEDController ledController;
PomodoroTimer pomodoro(ledController);

void setup() {
    ledController.init();
}

void loop() {
    pomodoro.startFocusSession();  // Tryb skupienia na 30 minut (zielony)
    pomodoro.resetTimer();         // Wyczyść diody po sesji
    delay(2000);                   // Przerwa między sesjami dla testów

    pomodoro.startBreakSession();  // Tryb przerwy na 5 minut (czerwony)
    pomodoro.resetTimer();         // Wyczyść diody po sesji
    delay(2000);                   // Przerwa między sesjami dla testów
}
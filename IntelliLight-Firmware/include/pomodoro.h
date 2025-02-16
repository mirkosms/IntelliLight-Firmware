#ifndef POMODORO_H
#define POMODORO_H

#include "led_controller.h"
#include <WebServer.h>

class PomodoroTimer {
public:
    PomodoroTimer(LEDController& ledController, WebServer& server);
    void startFocusSession(int focusMinutes = 30);
    void startBreakSession(int breakMinutes = 5);
    void resetTimer();
    void update();  // Nowa funkcja aktualizująca stan

private:
    LEDController& leds;
    WebServer& server;
    unsigned long startMillis = 0;
    int totalMinutes = 0;
    int elapsedMinutes = 0;
    int ledsPerMinute = 0;
    bool running = false;
    CRGB sessionColor;

    void sessionCompleteEffect();
};

#endif // POMODORO_H

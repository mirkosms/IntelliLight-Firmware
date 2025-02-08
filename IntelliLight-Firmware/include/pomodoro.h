#ifndef POMODORO_H
#define POMODORO_H

#include "led_controller.h"

class PomodoroTimer {
public:
    PomodoroTimer(LEDController& ledController);
    void startFocusSession(int focusMinutes = 30);
    void startBreakSession(int breakMinutes = 5);
    void resetTimer();

private:
    LEDController& leds;
    void countdown(int totalMinutes, CRGB color, int ledsPerMinute);
    void sessionCompleteEffect();
};

#endif // POMODORO_H

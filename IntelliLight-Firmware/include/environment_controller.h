#ifndef ENVIRONMENT_CONTROLLER_H
#define ENVIRONMENT_CONTROLLER_H

#include "sensors.h"
#include "led_controller.h"
#include "pomodoro.h"

class EnvironmentController {
public:
    EnvironmentController(Sensors &sensors, LEDController &ledController, PomodoroTimer &pomodoro,
                          bool &autoBrightnessEnabled, bool &motionEnabled, unsigned long &motionTimeout);
    void update();

private:
    Sensors &sensors;
    LEDController &ledController;
    PomodoroTimer &pomodoro;
    bool &autoBrightnessEnabled;
    bool &motionEnabled;
    unsigned long &motionTimeout;
    unsigned long lastMotionTime;

    void updateAutoBrightness();
    void updateMotionSensor();
};

#endif // ENVIRONMENT_CONTROLLER_H

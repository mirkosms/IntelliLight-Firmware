#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <WebServer.h>
#include "led_controller.h"
#include "pomodoro.h"
#include "sensors.h"
#include "wifi_manager.h"

void setupHTTPServer(WebServer &server, LEDController &ledController, PomodoroTimer &pomodoro, Sensors &sensors, WiFiManager &wifiManager, bool &autoBrightnessEnabled, bool &motionEnabled, unsigned long &motionTimeout);

#endif // HTTP_SERVER_H

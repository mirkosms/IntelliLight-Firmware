#include "led_controller.h"
#include "pomodoro.h"
#include "sensors.h"
#include "wifi_manager.h"
#include "http_server.h"
#include "environment_controller.h"
#include "config.h"
#include <WebServer.h>
#include <ESPmDNS.h>

const char* ssid = "FunBox2-99AC";
const char* password = "RBpc55EN";

LEDController ledController;
WebServer server(80);
PomodoroTimer pomodoro(ledController, server);
Sensors sensors;
WiFiManager wifiManager(ssid, password);
EnvironmentController env_controller(sensors, ledController, pomodoro,
                                     autoBrightnessEnabled, motionEnabled, motionTimeout);

void setup() {
    Serial.begin(9600);
    ledController.init();
    sensors.begin();
    wifiManager.connect();

    if (MDNS.begin("esp32")) {
        Serial.println("mDNS responder started");
    }

    // Konfiguracja serwera HTTP przez dedykowany moduł
    setupHTTPServer(server, ledController, pomodoro, sensors, wifiManager,
                      autoBrightnessEnabled, motionEnabled, motionTimeout);

    server.begin();
    Serial.println("HTTP server started!");
}

void loop() {
    server.handleClient();
    ledController.updateEffects();
    pomodoro.update();
    env_controller.update();
}

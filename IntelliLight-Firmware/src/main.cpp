#include "led_controller.h"
#include "pomodoro.h"
#include "sensors.h"
#include "wifi_manager.h"
#include "http_server.h"
#include "environment_controller.h"
#include "led_effects_manager.h"
#include "config.h"
#include <WebServer.h>
#include <ESPmDNS.h>

// const char* ssid = "FunBox2-99AC";
// const char* password = "RBpc55EN";

// const char* ssid = "TP-Link_ABA6";
// const char* password = "64116505";

const char* ssid = "DESKTOPSMS";
const char* password = "p12D092&";

// const char* ssid = "XDnet";
// const char* password = "koronawirus2k20";

LEDController ledController;
WebServer server(80);
PomodoroTimer pomodoro(ledController, server);
Sensors sensors;
WiFiManager wifiManager(ssid, password);
LEDEffectsManager ledEffectsMgr(ledController);
EnvironmentController envController(sensors, ledController, pomodoro,
                                     autoBrightnessEnabled, motionEnabled, motionTimeout);

void setup() {
    Serial.begin(9600);
    ledController.init();
    sensors.begin();
    wifiManager.connect();

    if (MDNS.begin("esp32")) {
        Serial.println("Uruchomiono usługę mDNS!");
    }

    setupHTTPServer(server, ledController, pomodoro, sensors, wifiManager,
                      autoBrightnessEnabled, motionEnabled, motionTimeout);

    server.begin();
    Serial.println("Serwer HTTP został uruchomiony!");
}

void loop() {
    server.handleClient();
    ledController.updateEffects();
    pomodoro.update();
    envController.update();
}

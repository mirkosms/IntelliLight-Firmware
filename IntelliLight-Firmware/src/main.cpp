#include "led_controller.h"
#include "pomodoro.h"
#include "sensors.h"
#include "wifi_manager.h"
#include "http_server.h"
#include <WebServer.h>
#include <ESPmDNS.h>

const char* ssid = "FunBox2-99AC";
const char* password = "RBpc55EN";

LEDController ledController;
WebServer server(80);
PomodoroTimer pomodoro(ledController, server);
Sensors sensors;
WiFiManager wifiManager(ssid, password);

bool autoBrightnessEnabled = false;  // Automatyczna regulacja jasności
bool motionEnabled = false;            // Domyślnie czujnik ruchu wyłączony
unsigned long motionTimeout = 60000;   // Timeout bezczynności (60s)
unsigned long lastMotionTime = 0;

void setup() {
    Serial.begin(9600);
    ledController.init();
    sensors.begin();
    wifiManager.connect();

    if (MDNS.begin("esp32")) Serial.println("mDNS responder started");

    // Konfiguracja serwera HTTP poprzez dedykowany moduł
    setupHTTPServer(server, ledController, pomodoro, sensors, wifiManager, autoBrightnessEnabled, motionEnabled, motionTimeout);

    server.begin();
    Serial.println("Serwer HTTP uruchomiony!");
}

void loop() {
    server.handleClient();
    ledController.updateEffects();
    pomodoro.update();

    if (autoBrightnessEnabled) {
        float lux = sensors.readLightLevel();
        ledController.setAutoBrightness(lux);
    }

    if (motionEnabled && !pomodoro.isRunning()) {
        bool motionDetected = sensors.readMotion();
        if (motionDetected) {
            lastMotionTime = millis();
            if (!ledController.isAnyEffectActive()) {
                if (ledController.getLastActiveEffect() == "none") {
                    ledController.setAll(0, 0, 255);
                } else {
                    String lastEffect = ledController.getLastActiveEffect();
                    if (lastEffect == "static") ledController.toggleStatic();
                    else if (lastEffect == "rainbow") ledController.toggleRainbow();
                    else if (lastEffect == "pulsing") ledController.togglePulsing();
                    else if (lastEffect == "night") ledController.toggleNightMode();
                    else if (lastEffect == "twinkle") ledController.toggleTwinkle();
                }
                ledController.setManualOverride(false);
            }
        } else if (millis() - lastMotionTime > motionTimeout) {
            Serial.println("Brak ruchu przez określony czas. Wyłączam LED.");
            ledController.clear();
            ledController.setManualOverride(false);
        }
    }
}

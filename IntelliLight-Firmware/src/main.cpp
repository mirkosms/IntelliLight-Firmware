#include "led_controller.h"
#include "pomodoro.h"
#include "sensors.h"
#include "wifi_manager.h"
#include <WebServer.h>

// Dane do Wi-Fi
const char* ssid = "FunBox2-99AC";
const char* password = "RBpc55EN";

LEDController ledController;
PomodoroTimer pomodoro(ledController);
Sensors sensors;
WiFiManager wifiManager(ssid, password);
WebServer server(80);

void handleRoot() {
    float temperature = sensors.readTemperature();
    float humidity = sensors.readHumidity();

    String response = "{";
    response += "\"temperature\": " + String(temperature, 2) + ",";
    response += "\"humidity\": " + String(humidity, 2);
    response += "}";

    server.send(200, "application/json", response);
}

void handleToggleEffect(String effectName) {
    if (effectName == "led") {
        ledController.toggleStatic();
        server.send(200, "text/plain", "Tryb LED zmieniony");
    } else if (effectName == "rainbow") {
        ledController.toggleRainbow();
        server.send(200, "text/plain", "Tryb Rainbow zmieniony");
    } else if (effectName == "pulsing") {
        ledController.togglePulsing();
        server.send(200, "text/plain", "Tryb Pulsing zmieniony");
    }
}

void setup() {
    Serial.begin(9600);
    ledController.init();
    sensors.begin();
    wifiManager.connect();

    server.on("/", handleRoot);
    server.on("/toggle/led", []() { handleToggleEffect("led"); });
    server.on("/toggle/rainbow", []() { handleToggleEffect("rainbow"); });
    server.on("/toggle/pulsing", []() { handleToggleEffect("pulsing"); });

    server.begin();
    Serial.println("Serwer HTTP uruchomiony!");
}

void loop() {
    server.handleClient();
    ledController.updateEffects();
}

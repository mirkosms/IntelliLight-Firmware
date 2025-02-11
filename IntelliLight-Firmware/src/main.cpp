#include "led_controller.h"
#include "pomodoro.h"
#include "sensors.h"
#include "wifi_manager.h"
#include <WebServer.h>

const char* ssid = "FunBox2-99AC";
const char* password = "RBpc55EN";

LEDController ledController;
PomodoroTimer pomodoro(ledController);
Sensors sensors;
WiFiManager wifiManager(ssid, password);
WebServer server(80);

void handleRoot() {
    String response = "{";
    response += "\"status\": \"ESP32 działa poprawnie\"";
    response += "}";
    server.send(200, "application/json", response);
}

void handleSensorData() {
    float temperature = sensors.readTemperature();
    float humidity = sensors.readHumidity();

    String response = "{";
    response += "\"temperature\": " + String(temperature, 2) + ",";
    response += "\"humidity\": " + String(humidity, 2);
    response += "}";

    server.send(200, "application/json", response);
}

void handleToggleEffect(String effectName, String param = "") {
    if (effectName == "led") {
        ledController.toggleStatic();
        server.send(200, "text/plain", "Tryb LED zmieniony");
    } else if (effectName == "rainbow") {
        ledController.toggleRainbow();
        server.send(200, "text/plain", "Tryb Rainbow zmieniony");
    } else if (effectName == "pulsing") {
        ledController.togglePulsing();
        server.send(200, "text/plain", "Tryb Pulsing zmieniony");
    } else if (effectName == "night") {
        ledController.toggleNightMode();
        server.send(200, "text/plain", "Tryb Night Mode zmieniony");
    } else if (effectName == "twinkle") {
        ledController.toggleTwinkle();
        server.send(200, "text/plain", "Tryb Twinkle zmieniony");
    } else if (effectName == "white") {
        ledController.toggleWhiteTemperature(param);
        server.send(200, "text/plain", "Temperatura barwowa ustawiona: " + param);
    } else {
        server.send(404, "text/plain", "Nieznany efekt");
    }
}

void setup() {
    Serial.begin(9600);
    ledController.init();
    sensors.begin();
    wifiManager.connect();

    server.on("/", handleRoot);
    server.on("/sensor", handleSensorData);
    server.on("/toggle/led", []() { handleToggleEffect("led"); });
    server.on("/toggle/rainbow", []() { handleToggleEffect("rainbow"); });
    server.on("/toggle/pulsing", []() { handleToggleEffect("pulsing"); });
    server.on("/toggle/night", []() { handleToggleEffect("night"); });
    server.on("/toggle/twinkle", []() { handleToggleEffect("twinkle"); });
    server.on("/toggle/white/neutral", []() { handleToggleEffect("white", "neutral"); });
    server.on("/toggle/white/cool", []() { handleToggleEffect("white", "cool"); });
    server.on("/toggle/white/warm", []() { handleToggleEffect("white", "warm"); });

    server.begin();
    Serial.println("Serwer HTTP uruchomiony!");
}

void loop() {
    server.handleClient();
    ledController.updateEffects();
}

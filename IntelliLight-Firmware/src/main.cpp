#include "led_controller.h"
#include "pomodoro.h"
#include "sensors.h"
#include "wifi_manager.h"
#include <WebServer.h>  // Do obsługi serwera HTTP

// Dane do Wi-Fi
const char* ssid = "FunBox2-99AC";
const char* password = "RBpc55EN";

LEDController ledController;
PomodoroTimer pomodoro(ledController);
Sensors sensors;
WiFiManager wifiManager(ssid, password);
WebServer server(80);  // Serwer na porcie 80

void handleRoot() {
    float temperature = sensors.readTemperature();
    float humidity = sensors.readHumidity();

    // Odpowiedź w formacie JSON
    String response = "{";
    response += "\"temperature\": " + String(temperature, 2) + ",";
    response += "\"humidity\": " + String(humidity, 2);
    response += "}";

    server.send(200, "application/json", response);
}

void handleLedOn() {
    ledController.setAll(0, 0, 255);  // Ustawienie niebieskiego koloru
    server.send(200, "text/plain", "LED włączone (niebieski)");
}

void handleLedOff() {
    ledController.clear();  // Wyłączenie LED-ów
    server.send(200, "text/plain", "LED wyłączone");
}

void handleRainbowOn() {
    ledController.rainbowEffect();  // Włącz efekt rainbow
    server.send(200, "text/plain", "Efekt rainbow włączony");
}

void handleRainbowOff() {
    ledController.stopRainbow();  // Wyłącz efekt rainbow
    server.send(200, "text/plain", "Efekt rainbow wyłączony");
}

void setup() {
    Serial.begin(9600);
    ledController.init();
    sensors.begin();
    wifiManager.connect();

    server.on("/", handleRoot);            // Obsługa odczytu z czujników
    server.on("/led/on", handleLedOn);     // Obsługa włączenia LED
    server.on("/led/off", handleLedOff);   // Obsługa wyłączenia LED
    server.on("/led/rainbow/on", handleRainbowOn);   // Włączenie efektu rainbow
    server.on("/led/rainbow/off", handleRainbowOff); // Wyłączenie efektu rainbow

    server.begin();  // Uruchomienie serwera po zarejestrowaniu handlerów
    Serial.println("Serwer HTTP uruchomiony!");
}

void loop() {
    server.handleClient();         // Obsługa żądań HTTP
    ledController.updateRainbow(); // Aktualizacja efektu rainbow (jeśli aktywny)
}

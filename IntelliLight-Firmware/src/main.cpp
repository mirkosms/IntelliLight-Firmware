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

    String response = "<h1>Odczyt z czujnika DHT22</h1>";
    response += "Temperatura: " + String(temperature) + " &deg;C<br>";
    response += "Wilgotność: " + String(humidity) + " %<br>";

    server.send(200, "text/html", response);
}

void setup() {
    Serial.begin(9600);
    ledController.init();
    sensors.begin();
    wifiManager.connect();

    server.on("/", handleRoot);  // Obsługa strony głównej
    server.begin();
    Serial.println("Serwer HTTP uruchomiony!");
}

void loop() {
    server.handleClient();  // Obsługa żądań HTTP
}

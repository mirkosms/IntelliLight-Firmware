#include "led_controller.h"
#include "pomodoro.h"
#include "sensors.h"
#include "wifi_manager.h"
#include <WebServer.h>
#include <ESPmDNS.h>

const char* ssid = "FunBox2-99AC";
const char* password = "RBpc55EN";

LEDController ledController;
WebServer server(80);
PomodoroTimer pomodoro(ledController, server);
Sensors sensors;
WiFiManager wifiManager(ssid, password);

bool autoBrightnessEnabled = false;  // Flaga dla trybu automatycznego dostosowywania jasności

void handleRoot() {
    server.send(200, "text/plain", "ESP32 działa poprawnie");
}

void handleSetBrightness() {
    if (!server.hasArg("value")) {
        server.send(400, "text/plain", "Błąd: Brak wartości jasności");
        return;
    }
    
    int brightness = server.arg("value").toInt();
    autoBrightnessEnabled = false;  // Wyłączenie automatycznej regulacji jasności
    ledController.setBrightness(brightness);
    server.send(200, "text/plain", "Ustawiono jasność: " + String(brightness));
}

void handleToggleAutoBrightness() {
    autoBrightnessEnabled = !autoBrightnessEnabled;
    server.send(200, "text/plain", "Automatyczna jasność: " + String(autoBrightnessEnabled ? "ON" : "OFF"));
}

void handleSensorData() {
    float temperature = sensors.readTemperature();
    float humidity = sensors.readHumidity();
    float lightIntensity = sensors.readLightLevel();  // Odczyt BH1750

    String response = "Temperatura: " + String(temperature, 1) + " °C\n";
    response += "Wilgotność: " + String(humidity, 1) + " %\n";
    response += "Natężenie światła: " + String(lightIntensity, 2) + " lx";

    server.send(200, "text/plain", response);
}


void handleGetIP() {
    server.send(200, "text/plain", WiFi.localIP().toString());
}

void handleToggleEffect(String effectName, String param = "") {
    if (effectName == "led") ledController.toggleStatic();
    else if (effectName == "rainbow") ledController.toggleRainbow();
    else if (effectName == "pulsing") ledController.togglePulsing();
    else if (effectName == "night") ledController.toggleNightMode();
    else if (effectName == "twinkle") ledController.toggleTwinkle();
    else if (effectName == "white") ledController.toggleWhiteTemperature(param);
    else {
        server.send(404, "text/plain", "Nieznany efekt");
        return;
    }

    server.send(200, "text/plain", "Tryb zmieniony: " + effectName);
}

void handlePomodoro() {
    if (!server.hasArg("mode")) {
        Serial.println("Błąd: Brak parametru mode w żądaniu!");
        server.send(400, "text/plain", "Błąd: Brak parametru mode");
        return;
    }

    String mode = server.arg("mode");
    Serial.println("Otrzymano żądanie Pomodoro: " + mode);

    if (mode == "focus") {
        pomodoro.startFocusSession(25);
    } else if (mode == "break") {
        pomodoro.startBreakSession(5);
    } else if (mode == "reset") {
        pomodoro.resetTimer();
    } else {
        Serial.println("Błąd: Nieznany tryb Pomodoro: " + mode);
        server.send(400, "text/plain", "Błąd: Nieznany tryb Pomodoro");
        return;
    }

    server.send(200, "text/plain", "Tryb Pomodoro: " + mode);
}

void setup() {
    Serial.begin(9600);
    ledController.init();
    sensors.begin();
    wifiManager.connect();

    if (MDNS.begin("esp32")) Serial.println("mDNS responder started");

    server.on("/", handleRoot);
    server.on("/sensor", handleSensorData);
    server.on("/getIP", handleGetIP);
    server.on("/brightness", handleSetBrightness);
    server.on("/toggle/autobrightness", handleToggleAutoBrightness);
    server.on("/toggle/led", []() { handleToggleEffect("led"); });
    server.on("/toggle/rainbow", []() { handleToggleEffect("rainbow"); });
    server.on("/toggle/pulsing", []() { handleToggleEffect("pulsing"); });
    server.on("/toggle/night", []() { handleToggleEffect("night"); });
    server.on("/toggle/twinkle", []() { handleToggleEffect("twinkle"); });
    server.on("/toggle/white/neutral", []() { handleToggleEffect("white", "neutral"); });
    server.on("/toggle/white/cool", []() { handleToggleEffect("white", "cool"); });
    server.on("/toggle/white/warm", []() { handleToggleEffect("white", "warm"); });
    server.on("/pomodoro", handlePomodoro);

    server.onNotFound([]() { server.send(404, "text/plain", "Błąd: Żądanie nieobsługiwane"); });

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
}
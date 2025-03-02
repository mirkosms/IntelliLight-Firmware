#include "http_server.h"
#include "led_effects_manager.h"
extern LEDEffectsManager ledEffectsMgr;

void handleRoot(WebServer &server) {
    server.send(200, "text/plain", "ESP32 działa poprawnie");
}

void handleSetBrightness(WebServer &server, LEDController &ledController, bool &autoBrightnessEnabled) {
    if (!server.hasArg("value")) {
        server.send(400, "text/plain", "Błąd: Brak wartości jasności");
        return;
    }
    int brightness = server.arg("value").toInt();
    autoBrightnessEnabled = false;
    ledController.setBrightness(brightness);
    server.send(200, "text/plain", "Ustawiono jasność: " + String(brightness));
}

void handleToggleAutoBrightness(WebServer &server, bool &autoBrightnessEnabled) {
    autoBrightnessEnabled = !autoBrightnessEnabled;
    server.send(200, "text/plain", "Automatyczna jasność: " + String(autoBrightnessEnabled ? "ON" : "OFF"));
}

void handleSensorData(WebServer &server, Sensors &sensors) {
    float temperature = sensors.readTemperature();
    float humidity = sensors.readHumidity();
    float lightIntensity = sensors.readLightLevel();
    String response = "Temperatura: " + String(temperature, 1) + " °C\n";
    response += "Wilgotność: " + String(humidity, 1) + " %\n";
    response += "Natężenie światła: " + String(lightIntensity, 2) + " lx";
    server.send(200, "text/plain", response);
}

void handleGetIP(WebServer &server) {
    server.send(200, "text/plain", WiFi.localIP().toString());
}

void handleToggleEffect(WebServer &server, LEDController &ledController, PomodoroTimer &pomodoro,
                          const String &effectName, const String &param) {
    if (pomodoro.isRunning()) {
        server.send(200, "text/plain", "Tryb Pomodoro jest aktywny. Proszę zresetować timer, aby zmienić tryb LED.");
        return;
    }
    ledEffectsMgr.toggle_effect(effectName, param);
    server.send(200, "text/plain", "Tryb zmieniony: " + effectName);
}

void handlePomodoro(WebServer &server, PomodoroTimer &pomodoro) {
    if (!server.hasArg("mode")) {
        server.send(400, "text/plain", "Błąd: Brak parametru mode");
        return;
    }
    String mode = server.arg("mode");
    if (mode == "focus") {
        pomodoro.startFocusSession(25);
    } else if (mode == "break") {
        pomodoro.startBreakSession(5);
    } else if (mode == "reset") {
        pomodoro.resetTimer();
    } else {
        server.send(400, "text/plain", "Błąd: Nieznany tryb Pomodoro");
        return;
    }
    server.send(200, "text/plain", "Tryb Pomodoro: " + mode);
}

void handleSetMotionTimeout(WebServer &server, unsigned long &motionTimeout) {
    if (!server.hasArg("seconds")) {
        server.send(400, "text/plain", "Błąd: Brak wartości timeout");
        return;
    }
    motionTimeout = server.arg("seconds").toInt() * 1000;
    server.send(200, "text/plain", "Ustawiono timeout na: " + String(motionTimeout / 1000) + " sekund");
}

void handleToggleMotionMode(WebServer &server, bool &motionEnabled) {
    motionEnabled = !motionEnabled;
    server.send(200, "text/plain", "Tryb czujnika ruchu: " + String(motionEnabled ? "ON" : "OFF"));
}

void setupHTTPServer(WebServer &server, LEDController &ledController, PomodoroTimer &pomodoro,
                     Sensors &sensors, WiFiManager &wifiManager, bool &autoBrightnessEnabled,
                     bool &motionEnabled, unsigned long &motionTimeout) {
    server.on("/", [&server]() { handleRoot(server); });
    server.on("/brightness", [&server, &ledController, &autoBrightnessEnabled]() { handleSetBrightness(server, ledController, autoBrightnessEnabled); });
    server.on("/toggle/autobrightness", [&server, &autoBrightnessEnabled]() { handleToggleAutoBrightness(server, autoBrightnessEnabled); });
    server.on("/sensor", [&server, &sensors]() { handleSensorData(server, sensors); });
    server.on("/getIP", [&server]() { handleGetIP(server); });
    server.on("/setMotionTimeout", [&server, &motionTimeout]() { handleSetMotionTimeout(server, motionTimeout); });
    server.on("/toggleMotionMode", [&server, &motionEnabled]() { handleToggleMotionMode(server, motionEnabled); });

    server.on("/toggle/led",       [&server, &ledController, &pomodoro]() { handleToggleEffect(server, ledController, pomodoro, "static", ""); });
    server.on("/toggle/rainbow",   [&server, &ledController, &pomodoro]() { handleToggleEffect(server, ledController, pomodoro, "rainbow", ""); });
    server.on("/toggle/pulsing",   [&server, &ledController, &pomodoro]() { handleToggleEffect(server, ledController, pomodoro, "pulsing", ""); });
    server.on("/toggle/night",     [&server, &ledController, &pomodoro]() { handleToggleEffect(server, ledController, pomodoro, "night", ""); });
    server.on("/toggle/twinkle",   [&server, &ledController, &pomodoro]() { handleToggleEffect(server, ledController, pomodoro, "twinkle", ""); });
    server.on("/toggle/white/neutral", [&server, &ledController, &pomodoro]() { handleToggleEffect(server, ledController, pomodoro, "white", "neutral"); });
    server.on("/toggle/white/cool",    [&server, &ledController, &pomodoro]() { handleToggleEffect(server, ledController, pomodoro, "white", "cool"); });
    server.on("/toggle/white/warm",    [&server, &ledController, &pomodoro]() { handleToggleEffect(server, ledController, pomodoro, "white", "warm"); });

    server.on("/pomodoro", [&server, &pomodoro]() { handlePomodoro(server, pomodoro); });

    server.onNotFound([&server]() { server.send(404, "text/plain", "Błąd: Żądanie nieobsługiwane"); });
}

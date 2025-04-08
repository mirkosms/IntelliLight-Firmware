#include "http_server.h"
#include "led_effects_manager.h"
extern LEDEffectsManager ledEffectsMgr;

void handleRoot(WebServer &server) {
    Serial.println("KOMENDA: Żądanie główne (/)");
    server.send(200, "text/plain", "ESP32 działa poprawnie");
}

void handleSetBrightness(WebServer &server, LEDController &ledController, bool &autoBrightnessEnabled) {
    Serial.println("KOMENDA: Ustaw jasność (/brightness)");
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
    Serial.println("KOMENDA: Przełącz automatyczną jasność (/toggle/autobrightness)");
    autoBrightnessEnabled = !autoBrightnessEnabled;
    server.send(200, "text/plain", "Automatyczna jasność: " + String(autoBrightnessEnabled ? "WŁĄCZONA" : "WYŁĄCZONA"));
}

void handleSensorData(WebServer &server, Sensors &sensors) {
    Serial.println("KOMENDA: Odczyt czujników (/sensor)");
    float temperature = sensors.readTemperature();
    float humidity = sensors.readHumidity();
    float lightIntensity = sensors.readLightLevel();
    String response = "Temperatura: " + String(temperature, 1) + " °C\n";
    response += "Wilgotność: " + String(humidity, 1) + " %\n";
    response += "Natężenie światła: " + String(lightIntensity, 2) + " lx";
    server.send(200, "text/plain", response);
}

void handleGetIP(WebServer &server) {
    Serial.println("KOMENDA: Pobierz adres IP (/getIP)");
    server.send(200, "text/plain", WiFi.localIP().toString());
}

void handleSetColor(WebServer &server, LEDController &ledController) {
    Serial.println("KOMENDA: Ustaw kolor LED (/setColor)");
    if (!server.hasArg("r") || !server.hasArg("g") || !server.hasArg("b")) {
        server.send(400, "text/plain", "Błąd: Brak jednego lub więcej parametrów (r, g, b)");
        return;
    }
    int r = server.arg("r").toInt();
    int g = server.arg("g").toInt();
    int b = server.arg("b").toInt();
    ledController.setCustomColor(r, g, b);
    ledController.setLastActiveEffect("custom");
    server.send(200, "text/plain", "Kolor LED ustawiony na: (" + String(r) + "," + String(g) + "," + String(b) + ")");
}

void handleToggleEffect(WebServer &server, LEDController &ledController, PomodoroTimer &pomodoro,
                        const String &effectName, const String &param) {
    Serial.print("KOMENDA: Przełącz efekt: ");
    Serial.println(effectName);
    if (pomodoro.isRunning()) {
        server.send(200, "text/plain", "Tryb Pomodoro jest aktywny. Zresetuj timer, aby zmienić efekt LED.");
        return;
    }
    ledEffectsMgr.toggle_effect(effectName, param);
    server.send(200, "text/plain", "Efekt zmieniony na: " + effectName);
}

void handlePomodoro(WebServer &server, PomodoroTimer &pomodoro) {
    Serial.println("KOMENDA: Sterowanie Pomodoro (/pomodoro)");
    if (!server.hasArg("mode")) {
        server.send(400, "text/plain", "Błąd: Brak parametru mode");
        return;
    }
    String mode = server.arg("mode");
    if (mode == "focus") {
        pomodoro.startFocusSession(30);
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
    Serial.println("KOMENDA: Ustaw czas bezruchu (/setMotionTimeout)");
    if (!server.hasArg("seconds")) {
        server.send(400, "text/plain", "Błąd: Brak wartości timeout");
        return;
    }
    motionTimeout = server.arg("seconds").toInt() * 1000;
    server.send(200, "text/plain", "Ustawiono czas bezruchu na: " + String(motionTimeout / 1000) + " sekund");
}

void handleToggleMotionMode(WebServer &server, bool &motionEnabled) {
    Serial.println("KOMENDA: Przełącz tryb czujnika ruchu (/toggleMotionMode)");
    motionEnabled = !motionEnabled;
    server.send(200, "text/plain", "Tryb czujnika ruchu: " + String(motionEnabled ? "WŁĄCZONY" : "WYŁĄCZONY"));
}

void setupHTTPServer(WebServer &server, LEDController &ledController, PomodoroTimer &pomodoro,
                     Sensors &sensors, WiFiManager &wifiManager, bool &autoBrightnessEnabled,
                     bool &motionEnabled, unsigned long &motionTimeout) {
    server.on("/", [&server]() { handleRoot(server); });
    server.on("/brightness", [&server, &ledController, &autoBrightnessEnabled]() {
        handleSetBrightness(server, ledController, autoBrightnessEnabled);
    });
    server.on("/toggle/autobrightness", [&server, &autoBrightnessEnabled]() {
        handleToggleAutoBrightness(server, autoBrightnessEnabled);
    });
    server.on("/sensor", [&server, &sensors]() {
        handleSensorData(server, sensors);
    });
    server.on("/getIP", [&server]() {
        handleGetIP(server);
    });
    server.on("/setMotionTimeout", [&server, &motionTimeout]() {
        handleSetMotionTimeout(server, motionTimeout);
    });
    server.on("/toggleMotionMode", [&server, &motionEnabled]() {
        handleToggleMotionMode(server, motionEnabled);
    });

    server.on("/toggle/led", [&server, &ledController, &pomodoro]() {
        handleToggleEffect(server, ledController, pomodoro, "static", "");
    });
    server.on("/toggle/rainbow", [&server, &ledController, &pomodoro]() {
        handleToggleEffect(server, ledController, pomodoro, "rainbow", "");
    });
    server.on("/toggle/pulsing", [&server, &ledController, &pomodoro]() {
        handleToggleEffect(server, ledController, pomodoro, "pulsing", "");
    });
    server.on("/toggle/night", [&server, &ledController, &pomodoro]() {
        handleToggleEffect(server, ledController, pomodoro, "night", "");
    });
    server.on("/toggle/twinkle", [&server, &ledController, &pomodoro]() {
        handleToggleEffect(server, ledController, pomodoro, "twinkle", "");
    });
    server.on("/toggle/white/neutral", [&server, &ledController, &pomodoro]() {
        handleToggleEffect(server, ledController, pomodoro, "white", "neutral");
    });
    server.on("/toggle/white/cool", [&server, &ledController, &pomodoro]() {
        handleToggleEffect(server, ledController, pomodoro, "white", "cool");
    });
    server.on("/toggle/white/warm", [&server, &ledController, &pomodoro]() {
        handleToggleEffect(server, ledController, pomodoro, "white", "warm");
    });

    server.on("/setColor", [&server, &ledController]() {
        handleSetColor(server, ledController);
    });

    server.on("/pomodoro", [&server, &pomodoro]() {
        handlePomodoro(server, pomodoro);
    });

    server.onNotFound([&server]() {
        Serial.print("BŁĄD: Nieznany adres endpointu: ");
        Serial.println(server.uri());
        server.send(404, "text/plain", "Błąd: Żądanie nieobsługiwane");
    });
}

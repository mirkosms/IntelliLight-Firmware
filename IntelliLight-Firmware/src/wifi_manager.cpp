#include "wifi_manager.h"

WiFiManager::WiFiManager(const char* ssid, const char* password)
    : ssid(ssid), password(password) {}

void WiFiManager::connect() {
    WiFi.begin(ssid, password);
    Serial.print("Łączenie z Wi-Fi");
    unsigned long startAttemptTime = millis();
    const unsigned long timeout = 20000; // Timeout 20 sekund

    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < timeout) {
        delay(500);
        Serial.print(".");
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nPołączono z Wi-Fi!");
        Serial.print("Adres IP: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("\nBłąd: Timeout połączenia z Wi-Fi.");
    }
}

String WiFiManager::getIP() {
    return WiFi.localIP().toString();
}

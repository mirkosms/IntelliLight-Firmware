#include "wifi_manager.h"

WiFiManager::WiFiManager(const char* ssid, const char* password)
    : ssid(ssid), password(password) {}

void WiFiManager::connect() {
    WiFi.begin(ssid, password);
    Serial.print("Łączenie z Wi-Fi");

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nPołączono z Wi-Fi!");
    Serial.print("Adres IP: ");
    Serial.println(WiFi.localIP());
}

String WiFiManager::getIP() {
    return WiFi.localIP().toString();
}

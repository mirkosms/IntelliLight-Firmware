#include "sensors.h"

Sensors::Sensors() : dht(DHT_PIN, DHT_TYPE) {}

void Sensors::begin() {
    dht.begin();
    Wire.begin();  // Inicjalizacja I2C
    lightMeter.begin();  // Inicjalizacja BH1750
    pinMode(PIR_PIN, INPUT);  // Inicjalizacja czujnika ruchu
}

float Sensors::readTemperature() {
    float temp = dht.readTemperature();
    if (isnan(temp)) {
        Serial.println("Błąd odczytu temperatury z DHT22.");
        return -999.0;
    }
    return temp;
}

float Sensors::readHumidity() {
    float humidity = dht.readHumidity();
    if (isnan(humidity)) {
        Serial.println("Błąd odczytu wilgotności z DHT22.");
        return -999.0;
    }
    return humidity;
}

float Sensors::readLightLevel() {
    return lightMeter.readLightLevel();  // Pobranie wartości natężenia światła w luksach
}

bool Sensors::readMotion() {
    return digitalRead(PIR_PIN) == HIGH;  // Zwraca true, jeśli wykryto ruch
}

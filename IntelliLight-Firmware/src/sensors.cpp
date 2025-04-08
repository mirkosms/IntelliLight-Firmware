#include "sensors.h"

Sensors::Sensors() : dht(DHT_PIN, DHT_TYPE) {}

void Sensors::begin() {
    dht.begin();
    Wire.begin();  // Inicjalizacja I2C
    lightMeter.begin();  // Inicjalizacja BH1750
    pinMode(PIR_PIN, INPUT);  // Inicjalizacja czujnika ruchu
}

float Sensors::readTemperature() {
    float temperature = dht.readTemperature();
    if (isnan(temperature)) {
        Serial.println("Błąd odczytu temperatury z DHT22.");
        return -999.0;
    }
    Serial.print("CZUJNIKI: Temperatura = ");
    Serial.println(temperature);
    return temperature;
}

float Sensors::readHumidity() {
    float humidity = dht.readHumidity();
    if (isnan(humidity)) {
        Serial.println("Błąd odczytu wilgotności z DHT22.");
        return -999.0;
    }
    Serial.print("CZUJNIKI: Wilgotność = ");
    Serial.println(humidity);
    return humidity;
}

float Sensors::readLightLevel() {
    float lightLevel = lightMeter.readLightLevel();
    Serial.print("CZUJNIKI: Natężenie światła = ");
    Serial.println(lightLevel);
    return lightLevel;
}

bool Sensors::readMotion() {
    static bool previousState = false;
    bool currentState = (digitalRead(PIR_PIN) == HIGH);
    if (currentState != previousState) {
        Serial.print("CZUJNIKI: Ruch = ");
        Serial.println(currentState ? "WYKRYTO" : "BRAK");
        previousState = currentState;
    }
    return currentState;
}

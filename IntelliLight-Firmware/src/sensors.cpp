#include "sensors.h"

Sensors::Sensors() : dht(DHT_PIN, DHT_TYPE) {}

void Sensors::begin() {
    dht.begin();
    Wire.begin();  // Inicjalizacja I2C
    lightMeter.begin();  // Inicjalizacja BH1750
}

float Sensors::readTemperature() {
    float temp = dht.readTemperature();
    if (isnan(temp)) {
        return -999.0;  // Wartość błędna, jeśli odczyt się nie powiódł
    }
    return temp;
}

float Sensors::readHumidity() {
    float humidity = dht.readHumidity();
    if (isnan(humidity)) {
        return -999.0;  // Wartość błędna, jeśli odczyt się nie powiódł
    }
    return humidity;
}

float Sensors::readLightLevel() {
    return lightMeter.readLightLevel();  // Pobranie wartości natężenia światła w luksach
}

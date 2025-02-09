#include "sensors.h"

Sensors::Sensors() : dht(DHT_PIN, DHT_TYPE) {}

void Sensors::begin() {
    dht.begin();
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

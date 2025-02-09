#ifndef SENSORS_H
#define SENSORS_H

#include <DHT.h>

#define DHT_PIN 4    // Pin GPIO do sygnału DATA
#define DHT_TYPE DHT22  // Typ czujnika: DHT22

class Sensors {
public:
    Sensors();
    void begin();
    float readTemperature();
    float readHumidity();
private:
    DHT dht;
};

#endif // SENSORS_H

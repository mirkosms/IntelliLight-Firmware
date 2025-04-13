#ifndef SENSORS_H
#define SENSORS_H

#include <DHT.h>
#include <BH1750.h>
#include <Wire.h>

#define DHT_PIN 4    // Pin GPIO do sygnału DATA
#define DHT_TYPE DHT22  // Typ czujnika: DHT22
#define PIR_PIN 13  // GPIO, do którego podłączony jest HC-SR501

class Sensors {
public:
    Sensors();
    void begin();
    float readTemperature();
    float readHumidity(); 
    float readLightLevel();
    bool readMotion();
    
private:
    DHT dht;
    BH1750 lightMeter;
};

#endif // SENSORS_H

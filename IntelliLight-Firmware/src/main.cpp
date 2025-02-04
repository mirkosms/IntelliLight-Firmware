#include <Arduino.h>

void setup() {
    Serial.begin(9600);
    Serial.println("Hello from IntelliLight Firmware!");
}

void loop() {
    Serial.println("Running...");
    delay(1000);
}

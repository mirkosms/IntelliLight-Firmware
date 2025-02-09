#include "led_controller.h"
#include "pomodoro.h"
#include "sensors.h"

LEDController ledController;
PomodoroTimer pomodoro(ledController);
Sensors sensors;  // Używamy klasy Sensors

void setup() {
    Serial.begin(9600);  // Monitor szeregowy do odczytu danych
    ledController.init();
    sensors.begin();     // Inicjalizacja czujnika
}

void loop() {
    float temperature = sensors.readTemperature();
    float humidity = sensors.readHumidity();

    Serial.print("Temperatura: ");
    Serial.print(temperature);
    Serial.println(" °C");

    Serial.print("Wilgotność: ");
    Serial.print(humidity);
    Serial.println(" %");

    delay(2000);  // Odczyt co 2 sekundy
}
